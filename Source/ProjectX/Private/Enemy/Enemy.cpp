// Fill out your copyright notice in the Description page of Project Settings.
// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "Components\SkeletalMeshComponent.h"
#include"Perception\PawnSensingComponent.h"
#include"GameFramework/CharacterMovementComponent.h"
#include"ProjectX\DebugMacros.h"	
#include"Components/AttributeComponent.h"
#include"HUD/HealthBarComponent.h"
#include"Items\Weapons\Weapon.h"
#include"GameMode\ArenaGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include"Items\ExperiencePoint.h"



// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("Healthbar"));
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("AISenseComponent"));
	HealthBarWidget->SetupAttachment(GetRootComponent());
	EEnemyState::EES_Patrolling;
	PawnSensing->SightRadius = 400.f;
	PawnSensing->SetPeripheralVisionAngle(45.f);
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add("Enemy");
	EnemyName = GetName();
	if (PawnSensing) PawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);
	InitializeEnemy();

	
}

void AEnemy::InitializeEnemy()
{
	EnemyController = Cast<AAIController>(GetController());
	HideHealthBar();
	MoveToTarget(PatrolTarget);
	SpawnDefaultWeapon();

}

void AEnemy::Die()
{
	if (IsDead()) return;
	Super::Die();
	GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Blue, EnemyName);
	AddKilledEnemy();
    SetEnemyDead();
	ClearAttackTimer();
	HideHealthBar();
	DisableCapsule();
	SetLifeSpan(DeathLifeSpan);
	GetCharacterMovement()->bOrientRotationToMovement = false;
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	SpawnExperience();

}

void AEnemy::SetEnemyDead()
{
	EnemyState = EEnemyState::EES_Dead;
}

void AEnemy::AddKilledEnemy()
{
	AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
	AArenaGameMode* ArenaGameMode = Cast<AArenaGameMode>(GameMode);
	if (ArenaGameMode) ArenaGameMode->KilledEnemiesNames.Add(EnemyName);
}

void AEnemy::SpawnExperience()
{
	UWorld* World = GetWorld();

	if (World && Experience && Attributes)
	{
		const FVector SpawnLocation = GetActorLocation() + FVector(10.f, 0.f, 30.f);
		AExperiencePoint* SpawnedExp = World->SpawnActor<AExperiencePoint>(Experience, SpawnLocation, GetActorRotation());
		if (SpawnedExp)
		{
			SpawnedExp->SetExp(Attributes->GetExperience());
		}		
	}	
}

void AEnemy::Attack()
{
	if (CombatTarget->ActorHasTag(FName("Dead")))
	{
		CombatTarget = nullptr;
	}

	if (CombatTarget == nullptr) return;
	EnemyState = EEnemyState::EES_Engaged;
	PlayAttackMontage();
}

void AEnemy::AttackEnd()
{
	EnemyState = EEnemyState::EES_NoState;
	CheckCombatTarget();
}

bool AEnemy::CanAttack()
{
	bool bCanAttack =
		IsInsideAttackRadius() &&
		!IsAttacking() &&
		!IsEngaged() &&
		!IsDead();
	return bCanAttack;
}

void AEnemy::HandleDamage(float DamageAmount)
{

  if(IsDead()) return;

	Super::HandleDamage(DamageAmount);

	if (Attributes && HealthBarWidget)
	{
		Attributes->ReciveDamage(DamageAmount);

		HealthBarWidget->SetHealthPercent(Attributes->HealthPercent());
	}

}



void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsDead()) return;
	if (EnemyState > EEnemyState::EES_Patrolling )
	{
		
		

		
		CheckCombatTarget();
	}
	else
	{
		CheckPatrolTarget();
	}

	
	/*if (GEngine)
	{
		FString EnemyIDString = FString::Printf(TEXT("Enemy ID: %d"), EnemyID);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, EnemyIDString);
	}*/
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{

	if (!IsDead())
	{
		HandleDamage(DamageAmount);
		CombatTarget = EventInstigator->GetPawn();

		if (IsInsideAttackRadius())
		{
			EnemyState = EEnemyState::EES_Attacking;
		}
		else if (IsOutsideAttackRadius())
		{
			ChaseTarget();
		}

		return DamageAmount;
	}
	else
	{
		return 0.f;
	}
	
}

void AEnemy::Destroyed()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}
}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint,AActor* Hitter)

{

	Super::GetHit_Implementation(ImpactPoint,Hitter);
	if (!IsDead()) ShowHealthBar();
	ClearPatrolTimer();
	ClearAttackTimer();
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	StopAttackMontage();

}

void AEnemy::SpawnDefaultWeapon()
{
	UWorld* World = GetWorld();
	if (World && WeaponClass)
	{
		AWeapon* DefaultWeapon = World->SpawnActor<AWeapon>(WeaponClass);
		DefaultWeapon->Equip(GetMesh(), FName("Sword"), this, this);
		EquippedWeapon = DefaultWeapon;
	}

}

void AEnemy::CheckPatrolTarget()
{

	if (InTargetRange(PatrolTarget, PatrolRadius))
	{
		PatrolTarget = ChoosePatrolTarget();

		const float WaitTime = FMath::RandRange(PatrolWaitTimeMin, PatrolWaitTimeMax);
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, WaitTime);

	}
}

void AEnemy::CheckCombatTarget()
{
	if (IsOutsideCombatRadius())
	{
		ClearAttackTimer();
		LoseInterest();
		if (!IsEngaged()) StartPatrolling();

		
	
	}
	else if (IsOutsideAttackRadius() && !IsChasing())
	{
		ClearAttackTimer();

		if (!IsEngaged()) ChaseTarget();
		
		
	}

	else if(CanAttack())
	{
		StartAttackTimer();
	}
}

void AEnemy::PatrolTimerFinished()
{
	MoveToTarget(PatrolTarget);
}

void AEnemy::HideHealthBar()
{
	HealthBarWidget->SetVisibility(false);

}

void AEnemy::ShowHealthBar()
{
	HealthBarWidget->SetVisibility(true);
}

void AEnemy::LoseInterest()
{
	CombatTarget = nullptr;
	HideHealthBar();
}

void AEnemy::StartPatrolling()
{
	EnemyState = EEnemyState::EES_Patrolling;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	MoveToTarget(PatrolTarget);
}

void AEnemy::ChaseTarget()
{	
	
	if (CombatTarget && CombatTarget->ActorHasTag(FName("Dead"))) {
		CombatTarget = nullptr;
		MoveToTarget(PatrolTarget);

	}


	EnemyState = EEnemyState::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	MoveToTarget(CombatTarget);

}

bool AEnemy::IsOutsideCombatRadius()
{
	return !InTargetRange(CombatTarget, CombatRadius);

}

bool AEnemy::IsOutsideAttackRadius()
{
	return !InTargetRange(CombatTarget, AttackRadius);
}

bool AEnemy::IsInsideAttackRadius()
{
	return InTargetRange(CombatTarget, AttackRadius);
}

bool AEnemy::IsChasing()
{
	return EnemyState == EEnemyState::EES_Chasing;
}

bool AEnemy::IsAttacking()
{
	return EnemyState == EEnemyState::EES_Attacking;
}

bool AEnemy::IsDead()
{
	return  EnemyState == EEnemyState::EES_Dead;
}

bool AEnemy::IsEngaged()
{
	return EnemyState == EEnemyState::EES_Engaged;
}

void AEnemy::ClearPatrolTimer()
{
	GetWorldTimerManager().ClearTimer(PatrolTimer);

}

void AEnemy::StartAttackTimer()
{
	EnemyState = EEnemyState::EES_Attacking;
	const float AttackTime = FMath::RandRange(AttackMin, AttackMax);
	GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
}

void AEnemy::ClearAttackTimer()
{
	GetWorldTimerManager().ClearTimer(AttackTimer);

}



bool AEnemy::InTargetRange(AActor* Target, double Radius)
{
	if (Target == nullptr) return false;
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	return DistanceToTarget <= Radius;
}

void AEnemy::MoveToTarget(AActor* Target)
{
	if (EnemyController == nullptr || Target == nullptr)return;
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(50.f);
	EnemyController->MoveTo(MoveRequest);
	
	
}

AActor* AEnemy::ChoosePatrolTarget()
{

	TArray<AActor*> ValidTargets;
	for (AActor* Target : PatrolTargets)
	{
		if (Target != PatrolTarget)
		{
			ValidTargets.AddUnique(Target);
		}
	}
	const int32 NumPatrolTargets = PatrolTargets.Num();
	if (NumPatrolTargets > 0)
	{
		const int32 TargetSelection = FMath::RandRange(0, NumPatrolTargets - 1);
		return PatrolTargets[TargetSelection];
		
	}


	return nullptr;
}

void AEnemy::PawnSeen(APawn* SeenPawn)
{
	const bool bShouldChaseTarget =
		EnemyState != EEnemyState::EES_Dead &&
		EnemyState != EEnemyState::EES_Chasing &&
		EnemyState < EEnemyState::EES_Attacking &&
		SeenPawn->ActorHasTag(FName("WarriorCharacter"));

	if (bShouldChaseTarget)
	{
		CombatTarget = SeenPawn;
		ClearPatrolTimer();
		ChaseTarget();

	}
	
}












