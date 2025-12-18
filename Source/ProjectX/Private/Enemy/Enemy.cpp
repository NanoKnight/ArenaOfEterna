// Fill out your copyright notice in the Description page of Project Settings.
// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "Components\SkeletalMeshComponent.h"
#include"Perception\PawnSensingComponent.h"
#include"GameFramework/CharacterMovementComponent.h"
#include"ProjectX\DebugMacros.h"	
#include"Components/AttributeComponent.h"
#include "Components\InventorySystem\InventoryComponent.h" ///*////
#include "Components/CapsuleComponent.h"
#include"HUD/HealthBarComponent.h"
#include"../WarriorCharacter.h"
#include"Items\Weapons\Weapon.h"
#include"GameMode\ArenaGameMode.h"
#include"CameraShakes\MainLegacyCameraShake.h"
#include "Kismet/GameplayStatics.h"
#include"Items\EnemySpawner.h"
#include "AIController.h"
#include "Items\SpawnManager.h"
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
	AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
	AArenaGameMode* ArenaGameMode = Cast<AArenaGameMode>(GameMode);
	ArenaGameMode->IncrementEnemyAlive();
	
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		ACharacter* PlayerCharacter = Cast<ACharacter>(PlayerController->GetPawn());
		if (PlayerCharacter)
		{
			WarriorCharacter = Cast<AWarriorCharacter>(PlayerCharacter);
			
		}
	}
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
	AddKilledEnemy();
    SetEnemyDead();
	ClearAttackTimer();
	HideHealthBar();
	DisableCapsule();
	GetCharacterMovement()->bOrientRotationToMovement = false;
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	IncreaseQuestKillCount();
	SetLifeSpan(DeathLifeSpan);
	DestroyEquipItems();
	SpawnEquipedItemsToWorld();
	GetWorld()->GetTimerManager().SetTimer(SpawnExperienceTimer, this, &AEnemy::SpawnExperience, 2.f);
	
	
	
	
	TArray<AActor*>FoundSpawners;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemySpawner::StaticClass(), FoundSpawners);
	for (AActor* Actor : FoundSpawners)
	{
		AEnemySpawner* SpawnerActor = Cast<AEnemySpawner>(Actor);
		SpawnerActor->OnEnemyKilled();
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan, FString::Printf(TEXT("EnemyKilled")));
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUFunction(this, FName("RespawnInfiniteEnemy"), SpawnerActor);
		GetWorld()->GetTimerManager().SetTimer(RespawnInfiniteEnemyTimer, TimerDelegate, 3.f, false);

	}
	AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
	AArenaGameMode* ArenaGameMode = Cast<AArenaGameMode>(GameMode);
	//ArenaGameMode->DecrementEnemyAlive();
	


}

void AEnemy::SpawnEquipedItemsToWorld()
{
	int32 EquippedItemCount = InventoryComponent->EquippedItems.Num();

	for (int32 i = 0; i < EquippedItemCount; i++)

	{

		int32 RandomNumber = FMath::RandRange(1, ItemSpawnRate);
		if (RandomNumber == 1 || SpecialCharacter == true)
		{
			FInventoryStruct& EquipItems = InventoryComponent->EquippedItems[i];
			ABaseItem* SpawnedItem = GetWorld()->SpawnActor<ABaseItem>(EquipItems.ItemClass);
			if (SpawnedItem)
			{
				FVector SpawnLocation = GetActorLocation() + FVector(i * 100, 0, 0);
				SpawnedItem->SetActorLocation(SpawnLocation);
				SpawnedItem->GetItemMesh()->SetSimulatePhysics(true);
			}
		}
		

	}
}

void AEnemy::DestroyEquipItems()
{
	for (ABaseItem* Item : ItemsToEquip)
	{
		Item->Destroy();

	}
}

void AEnemy::RespawnInfiniteEnemy(AEnemySpawner* SpawnerActor)
{
	if (SpawnerActor->Loop && SpawnerActor->EnemyAliveForLoop == 0)
	{
		SpawnerActor->SpawnEnemy(SpawnerActor->EnemySpawnCount);
	}
}

void AEnemy::IncreaseQuestKillCount()
{
	if (WarriorCharacter && !InfiniteEnemy && WarriorCharacter->CurrentQuest.QuestType == EQuestType::KillEnemies)
	{

		WarriorCharacter->CurrentQuest.CurrentKillCount++;
	}
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
	if (!GetWorld() || !Experience)
	{
		return;
	}

	if (World && Experience && Attributes)
	{
		const FVector SpawnLocation = GetActorLocation() + FVector(10.f, 0.f, 30.f);
		AExperiencePoint* SpawnedExp = World->SpawnActor<AExperiencePoint>(Experience, SpawnLocation, GetActorRotation());
		if (SpawnedExp)
		{
			SpawnedExp->SetExp(ExperiencePoint);
		}		
	}	
}

void AEnemy::Attack()
{
	if (EnemyState == EEnemyState::EAS_Stun)
	{
		CombatTarget = nullptr;
	}
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
    HealthBarWidget->SetHealthPercent(Attributes->HealthPercent());

}

void AEnemy::PlayHitSound(const FVector& ImpactPoint)
{
	Super::PlayHitSound(ImpactPoint);
}



void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (EnemyState > EEnemyState::EES_Patrolling )
	{
		CheckCombatTarget();
	}
	else
	{
		CheckPatrolTarget(); 
	}

	if (Ragdoll)
	{
		FVector newLoc = GetMesh()->GetSocketLocation("pelvis");
		GetCapsuleComponent()->SetWorldLocation(newLoc);

		FVector SocketLoc = GetMesh()->GetSocketLocation("pelvis");
		FRotator SocketRot = GetMesh()->GetSocketRotation("pelvis");
		FHitResult OutHit;
		FVector Start = SocketLoc;
		FVector ForwardVector = GetActorForwardVector();
		FVector End = Start + (GetMesh()->GetForwardVector() * -25);
		FCollisionQueryParams CollisionParams;
		CollisionParams.AddIgnoredActor(this);
		CollisionParams.AddIgnoredActor(WarriorCharacter);
		FHitResult HitResult;
		bool Bhit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams);
		//DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1, 0, 1);
		//DrawDebugPoint(GetWorld(), HitResult.Location, 10, FColor::Green, false, 1);
		if (Bhit)
		{
			FrontAnim = true;
		}
	}


	
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{

	
	if (!IsDead())
	{
		HandleDamage(DamageAmount);
		CombatTarget = EventInstigator->GetPawn();
		
		if (IsInsideAttackRadius() && EnemyState != EEnemyState::EAS_Stun)
		{
			
			EnemyState = EEnemyState::EES_Attacking;
		}
		else if (IsOutsideAttackRadius())
		{
			ChaseTarget();
		}
		if (Attributes->GetHealth() == 0 )
		{
			Die();
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

void AEnemy::SetRagdoll()
{
	Ragdoll = true;
	EnemyOutlineMesh->SetVisibility(false);
	GetMesh()->SetSimulatePhysics(true);
	CombatTarget = nullptr;
	EnemyOutlineMesh->SetSimulatePhysics(true);
	EnemyOutlineMesh->SetVisibility(false);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	EnemyState = EEnemyState::EAS_Stun;
	GetWorld()->GetTimerManager().SetTimer(HideHealthBarTimer, this, &AEnemy::HideHealthBar, 1.f);
	GetWorld()->GetTimerManager().SetTimer(RagdollTimer, this, &AEnemy::ResetRagdoll, 3.f);
	

}

void AEnemy::ResetRagdoll()
{
	if (IsDead())return;
	GetMesh()->SetSimulatePhysics(false);
	EnemyOutlineMesh->SetSimulatePhysics(false);
	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	GetMesh()->AttachToComponent(CapsuleComp, FAttachmentTransformRules::SnapToTargetIncludingScale);
	EnemyOutlineMesh->AttachToComponent(CapsuleComp, FAttachmentTransformRules::SnapToTargetIncludingScale);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	UAnimInstance* AnimInstance1 = GetMesh()->GetAnimInstance();
	UAnimInstance* AnimInstance2 = EnemyOutlineMesh->GetAnimInstance();
	
	
	if (AnimInstance1)
	{
		AnimInstance1->Montage_Play(SkillDamageMontage);

		if (FrontAnim)
		{
			AnimInstance1->Montage_JumpToSection("Front",SkillDamageMontage);
		}

		else
		{
			AnimInstance1->Montage_JumpToSection("Back",SkillDamageMontage);

		}
	  
	}
	if (AnimInstance2)
	{
		AnimInstance2->Montage_Play(SkillDamageMontage);
		if (FrontAnim)
		{
			AnimInstance2->Montage_JumpToSection("Front", SkillDamageMontage);
		}
		else
		{
			AnimInstance2->Montage_JumpToSection("Back", SkillDamageMontage);

		}
		//AnimInstance2->Montage_Play(SkillDamageMontage);
	}
	
	Ragdoll = false;

	FRotator MeshRot(0,-90,0);
	FVector MeshLoc(0, 0, -90);
	GetMesh()->SetRelativeLocationAndRotation(MeshLoc, MeshRot);
	EnemyOutlineMesh->SetRelativeLocationAndRotation(MeshLoc, MeshRot);

}


void AEnemy::ResetEnemyState()
{
	EnemyState = EEnemyState::EES_Patrolling;
	EnemyOutlineMesh->SetVisibility(true);
	if (CombatTarget)
	{
		ChaseTarget();
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

void AEnemy::SkillHit(const FVector& ImpactPoint, AActor* Hitter)
{
	if (!IsDead()) ShowHealthBar();
	ClearPatrolTimer();
	ClearAttackTimer();
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	PlayHitSound(ImpactPoint);
	StopAttackMontage();
	EnemyState = EEnemyState::EAS_Stun;
	if (Attributes->HealthPercent() <= 0)
	{
		SetEnemyDead();
	}

    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
	PlayerController->PlayerCameraManager->StartCameraShake(UMainLegacyCameraShake::StaticClass());
	}
		
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

	if (IsDead()) return;
	if (EnemyState == EEnemyState::EAS_Stun)
	{
		return;
	}

	if (InTargetRange(PatrolTarget, PatrolRadius))
	{
		PatrolTarget = ChoosePatrolTarget();

		const float WaitTime = FMath::RandRange(PatrolWaitTimeMin, PatrolWaitTimeMax);
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, WaitTime);

	}
}

void AEnemy::CheckCombatTarget()
{
	if (IsDead()) return;
	if (EnemyState == EEnemyState::EAS_Stun)
	{
		return;
	}

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
	if (EnemyState == EEnemyState::EAS_Stun) return;
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

bool AEnemy::IsStun()
{
	return EnemyState == EEnemyState::EAS_Stun;
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












