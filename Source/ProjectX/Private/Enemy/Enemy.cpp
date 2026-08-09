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
#include"Interfaces\CombatSoundInterface.h"
#include"Items\Weapons\Weapon.h"
#include"GameMode\ArenaGameMode.h"
#include"CameraShakes\MainLegacyCameraShake.h"
#include "Kismet/GameplayStatics.h"
#include"Components\AudioComponent.h"
#include"Items\EnemySpawner.h"
#include"Enemy\CombatDirector.h"
#include "AIController.h"
#include"NavigationSystem.h"
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
	ParryWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PerryWidget"));
	ParryWidget->SetupAttachment(GetRootComponent());
	PawnSensing->SightRadius = 1800.f;
	PawnSensing->SetPeripheralVisionAngle(120.f);
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;


}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	CombatDirector = Cast<ACombatDirector>(UGameplayStatics::GetActorOfClass(GetWorld(),
		ACombatDirector::StaticClass()));



	Tags.Add("Enemy");

	 //InitializeEquipItems();
	EnemyName = GetName();
	if (PawnSensing)
	{
		PawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);
		PawnSensing->OnHearNoise.AddDynamic(this, &AEnemy::PawnHeard);

	}
		
	InitializeEnemy();

	//GetWorld()->GetTimerManager().SetTimer(RandomMoveTimer, this, &AEnemy::MoveToRandomLocation, 2.f, true);
	
	AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
	if (GameMode)
	{
		AArenaGameMode* ArenaGameMode = Cast<AArenaGameMode>(GameMode);
		if (ArenaGameMode)
		{
			ArenaGameMode->IncrementEnemyAlive();

		}

	}
	
	
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
	if (CombatDirector) CombatDirector->ReleaseAttacker(this);
	

	SetRagdoll();
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	AddKilledEnemy();
    SetEnemyDead();
	ClearAttackTimer();
	HideHealthBar();
	DisableCapsule();
	GetCharacterMovement()->bOrientRotationToMovement = false;
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
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUFunction(this, FName("RespawnInfiniteEnemy"), SpawnerActor);
		GetWorld()->GetTimerManager().SetTimer(RespawnInfiniteEnemyTimer, TimerDelegate, 3.f, false);

	}

	
	if (SeenPawnRef)
	{
		if (ICombatSoundInterface* CombatInterface = Cast<ICombatSoundInterface>(SeenPawnRef))
		{
			CombatInterface->EnemyStoppedChasing();
		}
	}
	
	

}

void AEnemy::InitializeEquipItems()
{
	Super::InitializeEquipItems();
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
	if (SpawnerActor->Loop && SpawnerActor->EnemyAlive == 0)
	{
		SpawnerActor->SpawnEnemy(SpawnerActor->EnemySpawnCount);
	}
}

void AEnemy::IncreaseQuestKillCount()
{
	if (IgnoreEnemyCount == true) return;

	if (WarriorCharacter && !InfiniteEnemy && WarriorCharacter->CurrentQuest.QuestType == EQuestType::KillEnemies)
	{

		WarriorCharacter->CurrentQuest.CurrentKillCount++;
	}

	if (WarriorCharacter && !InfiniteEnemy && WarriorCharacter->CurrentQuest.QuestType == EQuestType::DestroyBoss)
	{

		WarriorCharacter->CurrentQuest.CurrentKillCount++;
	}

}

void AEnemy::SetEnemyDead()
{
	EnemyState = EEnemyState::EES_Dead;
	ParryWidget->SetVisibility(false);
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

	PlayerCantParry();

	if (IsDead() ||
		EnemyState == EEnemyState::EAS_Stun ||
		EnemyState == EEnemyState::EAS_Freezed)
	{
		return;
	}

	if (!IsValid(CombatTarget))
	{
		EnemyState = EEnemyState::EES_NoState;
		return;
	}

	AWarriorCharacter* WarChar =
		Cast<AWarriorCharacter>(CombatTarget);

	if (!IsValid(WarChar))
	{
		EnemyState = EEnemyState::EES_NoState;
		return;
	}

	if (WarChar->UnTouchable)
	{
		EnemyState = EEnemyState::EES_NoState;
		CheckCombatTarget();
		return;
	}

	if (CombatTarget->ActorHasTag(FName("Dead")))
	{
		CombatTarget = nullptr;
	}

	EnemyState = EEnemyState::EES_Engaged;
	PlayAttackMontage();

	
}

void AEnemy::PlayerCantParry()
{
	
	CanParry = false;
	ParryWidget->SetVisibility(false);

}



void AEnemy::AttackEnd()
{
	EnemyState = EEnemyState::EES_NoState;
	CheckCombatTarget();

}

bool AEnemy::CanAttack()
{
	bool bCanAttack =
		HasAttackPermission() &&
		IsInsideAttackRadius() &&
		!IsAttacking() &&
		!IsEngaged() &&
		EnemyState != EEnemyState::EAS_Freezed &&
		!bRecoveringFromHit &&
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
	
	/*
	if (EnemyState != EEnemyState::EES_Patrolling )
	{
		CheckCombatTarget();
	}
	else
	{
		CheckPatrolTarget(); 
	
	}
	if (GetCharacterMovement()->Velocity.Size() == 0.f && EnemyState == EEnemyState::EES_Chasing
		&& CombatTarget == nullptr)
	{

		IdleTime += DeltaTime;

		if (IdleTime >= 2.f)
		{
			BackPatrol();
		}

	}*/

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
			ResetEnemyState();
		}
		else if (IsOutsideAttackRadius())
		{
			ChaseTarget();
		}
		if (Attributes->GetHealth() == 0 )
		{
			Die();
		}

		if (CombatDirector->CurrentAttacker != this)
		{
			CombatDirector->CurrentAttacker = this;

		}
		if (CombatDirector->CurrentAttacker == this)
		{
			TakedHit++;

			FTimerHandle HitCountTimer;
			if (!CombatTarget) return 0.f;

			if (TakedHit >= 2)
			{
				CombatDirector->SelectNextAttacker();
				FVector deneme = CombatTarget->GetActorLocation() -
					CombatTarget->GetActorForwardVector() * 150;
				MoveToSurroundLocation(deneme);
			
			}
			GetWorld()->GetTimerManager().SetTimer(HitCountTimer, this, &AEnemy::ResetTakedHit, 3.f);
			

		}
		return DamageAmount;
	}
	else
	{
		return 0.f;
	}
	
}

void AEnemy::ResetTakedHit()
{
	TakedHit = 0;
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
	GetMesh()->SetSimulatePhysics(true);
	CombatTarget = nullptr;
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	

}

void AEnemy::SetStun()
{
	EnemyState = EEnemyState::EAS_Stun;
	GetWorld()->GetTimerManager().SetTimer(HideHealthBarTimer, this, &AEnemy::HideHealthBar, 1.f);
	GetWorld()->GetTimerManager().SetTimer(RagdollTimer, this, &AEnemy::ResetRagdoll, 3.f);


}





void AEnemy::ParryReset()
{
	GetWorld()->GetTimerManager().SetTimer(ParryResetTimer, this, &AEnemy::ResetEnemyState, 2.f);


}



void AEnemy::SetEnemyFreeze()
{
	EnemyState = EEnemyState::EAS_Freezed;
	OldMat = GetMesh()->GetMaterial(0);
	GetMesh()->SetMaterial(0,FreezeMat);
	this->GetCharacterMovement()->StopMovementImmediately();
	GetWorld()->GetTimerManager().SetTimer(OutFreezeTimer, this, &AEnemy::OutFreeze, 3.f);
}

void AEnemy::OutFreeze()
{
	EnemyState = EEnemyState::EES_NoState;
	GetMesh()->SetMaterial(0,OldMat);

}

void AEnemy::ResetRagdoll()
{
	if (IsDead())return;
	GetMesh()->SetSimulatePhysics(false);
	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	GetMesh()->AttachToComponent(CapsuleComp, FAttachmentTransformRules::SnapToTargetIncludingScale);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	
	
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(SkillDamageMontage);

		if (FrontAnim)
		{
			AnimInstance->Montage_JumpToSection("Front",SkillDamageMontage);
		}

		else
		{
			AnimInstance->Montage_JumpToSection("Back",SkillDamageMontage);

		}
	  
	}
	
	Ragdoll = false;
	FRotator MeshRot(0,-90,0);
	FVector MeshLoc(0, 0, -90);
	GetMesh()->SetRelativeLocationAndRotation(MeshLoc, MeshRot);


	if (CombatTarget)
	{
		EnemyState = EEnemyState::EES_Chasing;
	}
	else
	{
		EnemyState = EEnemyState::EES_NoState;

	}

}

void AEnemy::BackPatrol()
{
	MoveToTarget(PatrolTarget);
	CombatTarget = nullptr;
	IdleTime = 0;
}

void AEnemy::MoveToSurroundLocation(const FVector& Location)
{
	/*************CONTROLS******/
	if (!IsValid(EnemyController))
	{
		return;
	}
	/*****************************/



	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalLocation(Location);
	MoveRequest.SetAcceptanceRadius(50.f);

	EnemyController->MoveTo(MoveRequest);
}


void AEnemy::ResetEnemyState()
{
	EnemyState = EEnemyState::EES_NoState;


	if (CombatTarget)
	{
		ChaseTarget();
	}


}

void AEnemy::GetParried()
{
	if (IsDead())return;
	ClearAttackTimer();
	ClearPatrolTimer();
	StopAttackMontage();
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	if (EnemyController)
	{
		EnemyController->StopMovement();
	}


	CanParry = false;
	if (ParryWidget)
	{
		ParryWidget->SetVisibility(false);
	}
	EnemyState = EEnemyState::EAS_Stun;
	GetWorldTimerManager().ClearTimer(ParryResetTimer);

	GetWorld()->GetTimerManager().SetTimer(ParryResetTimer, this, &AEnemy::EndParried, 2.f);
}





void AEnemy::GetHit_Implementation(const FVector& ImpactPoint,AActor* Hitter)
{
	Super::GetHit_Implementation(ImpactPoint,Hitter);
	GetWorldTimerManager().ClearTimer(AttackTimer);
	GetWorldTimerManager().ClearTimer(ParryStartTimer);
	GetWorldTimerManager().ClearTimer(RecoveryHitTimer);
	PlayerCantParry();
	bRecoveringFromHit = true;
	
	GetWorld()->GetTimerManager().SetTimer
	(RecoveryHitTimer, this, &AEnemy::EndRecoveryHit, 0.7f);


	if (!IsDead()) ShowHealthBar();
	ParryWidget->SetVisibility(false);
	ClearPatrolTimer();
	ClearAttackTimer();
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	StopAttackMontage();
	FVector Distance = GetActorLocation() - Hitter->GetActorLocation();
	Distance.Normalize(0.1);
	FVector LaunchLoc = FVector(Distance.X, Distance.Y, 0.f) *500;
	LaunchCharacter(LaunchLoc,false,false);


}

void AEnemy::EndRecoveryHit()
{
	bRecoveringFromHit = false;
}

void AEnemy::SkillHit(const FVector& ImpactPoint, AActor* Hitter)
{
	if (!IsDead()) ShowHealthBar();
	ClearPatrolTimer();
	ClearAttackTimer();
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	PlayHitSound(ImpactPoint);
	StopAttackMontage();


	if (EnemyType == EEnemyType::EET_Boss)	DirectionalHit(Hitter->GetActorLocation());	
	if (EnemyType == EEnemyType::EET_Enemy) EnemyState = EEnemyState::EAS_Stun;

	
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
	if (EnemyState == EEnemyState::EAS_Stun) return;
	

	if (IsOutsideCombatRadius())
	{
		ClearAttackTimer();
		LoseInterest();
		CheckCombatMusic();
		if (!IsEngaged()) StartPatrolling();

	}
	else if (IsOutsideAttackRadius() && !IsChasing())
	{
		ClearAttackTimer();
		EnemyState = EEnemyState::EES_NoState;

		if (!IsEngaged()) ChaseTarget();
		
		
	}

	else if(CanAttack())
	{
		StartAttackTimer();
		
	}
}

void AEnemy::CheckCombatMusic()
{
	if (Chased && SeenPawnRef)
	{
		if (ICombatSoundInterface* CombatInterface = Cast<ICombatSoundInterface>(SeenPawnRef))
		{
			CombatInterface->EnemyStoppedChasing();
			Chased = false;
			SeenPawnRef = nullptr;
		}
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
	if (CombatDirector) CombatDirector->ReleaseAttacker(this);
	
	
	HideHealthBar();
}

void AEnemy::StartPatrolling()
{
	if (PatrolTarget)
	{
		EnemyState = EEnemyState::EES_Patrolling;
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		MoveToTarget(PatrolTarget);
	}
	
}

void AEnemy::EndParried()
{

	if (IsDead())return;
	CanParry = false;
	EnemyState = EEnemyState::EES_NoState;

	if (IsValid(CombatTarget))
	{
		CheckCombatTarget();
	}
	else
	{
		StartPatrolling();
	}
}

void AEnemy::ChaseTarget()
{	


	/****************    CONTROLS   ***********************/
    if (Attributes->GetStamina() <= 0 && EnemyType == EEnemyType::EET_Boss ) EnemyState = EEnemyState::EAS_Stun;
	if (EnemyState == EEnemyState::EAS_Stun || EnemyState == EEnemyState::EAS_Freezed 
		|| EnemyState == EEnemyState::EES_Attacking) return;

	if (CombatTarget && CombatTarget->ActorHasTag(FName("Dead"))) {
		CombatTarget = nullptr;
		MoveToTarget(PatrolTarget);
	}
	if (!CombatTarget || !CombatDirector->CurrentAttacker) return;

	/************************************************************/

	if (CombatDirector->CurrentAttacker == this)
	{
		EnemyState = EEnemyState::EES_Chasing;
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
		MoveToTarget(CombatTarget);
	}
	else
	{

		//EnemyState = EEnemyState::EES_Surround;
		EnemyState = EEnemyState::EES_NoState;
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
		FVector TargetLoc = CombatDirector->CurrentAttacker->GetActorLocation() -
		CombatDirector->CurrentAttacker->GetActorForwardVector() * 200;
		MoveToSurroundLocation(TargetLoc);
		
		


	
	}
	
	

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

	if (CombatDirector->CurrentAttacker == this)
	{
		if (GetWorldTimerManager().IsTimerActive(AttackTimer))
		{
			return;
		}

		EnemyState = EEnemyState::EES_Attacking;
		const float AttackTime = FMath::RandRange(AttackMin, AttackMax);
	
		GetWorldTimerManager().SetTimer(ParryStartTimer, this, &AEnemy::PlayerCanParry, AttackTime / 2);


			

		GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
	
	}
	else
	{

		EnemyState = EEnemyState::EES_Surround;
	}
	



}

void AEnemy::PlayerCanParry()
{
		CanParry = true;
		ParryWidget->SetVisibility(true);
}



void AEnemy::ClearAttackTimer()
{
	GetWorldTimerManager().ClearTimer(AttackTimer);
	PlayerCantParry();

}


void AEnemy::UpdateCombatMovement()
{

	if (!IsValid(CombatDirector) ||
		!IsValid(EnemyController) ||
		!IsValid(CombatTarget))
	{
		return;
	}

	if (HasAttackPermission())
	{
		return;
	}

	const FVector SurroundLoc = CombatDirector->GetSurroundLocation(this);

	if (!SurroundLoc.IsNearlyZero())
	{
		EnemyController->MoveToLocation(SurroundLoc, 50.f);

		

		
	}

}



bool AEnemy::InTargetRange(AActor* Target, double Radius)
{
	if (Target == nullptr) return false;
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	return DistanceToTarget <= Radius;
}

bool AEnemy::HasAttackPermission() const
{
	
	return CombatDirector &&
		CombatDirector->CurrentAttacker == this;
}

void AEnemy::MoveToTarget(AActor* Target)
{
	if (EnemyState == EEnemyState::EAS_Stun) return;
	
	if (EnemyController == nullptr || Target == nullptr)return;
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(75.f);
	EnemyController->MoveTo(MoveRequest);
	
	
}

void AEnemy::MoveToRandomLocation()
{

	
	if (!EnemyController) return;
	if (PatrolTarget)return;
	if (CombatTarget)return;

	FVector RandomLocatation = GetActorLocation();
	RandomLocatation.X += FMath::RandRange(-1000.f, 1000.f);
	RandomLocatation.Y += FMath::RandRange(-1000.f, 1000.f);


	FVector Origin = GetActorLocation();
	float Radius = 500.f; 
	FNavLocation NavLocation;

	if (EnemyController)
	{
		if (UNavigationSystemV1::GetCurrent(GetWorld())->GetRandomPointInNavigableRadius(Origin, Radius, NavLocation)) {
			FAIMoveRequest MoveRequest;
			MoveRequest.SetGoalLocation(RandomLocatation);
			MoveRequest.SetAcceptanceRadius(50.f);
			EnemyController->MoveTo(MoveRequest);

		}
	}
	

	

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



	if (EnemyState == EEnemyState::EAS_Freezed)return;

	SeenPawnRef = SeenPawn;


	const bool bShouldChaseTarget =
		EnemyState != EEnemyState::EES_Dead &&
		EnemyState != EEnemyState::EES_Chasing &&
		SeenPawn->ActorHasTag(FName("WarriorCharacter"));

	if (bShouldChaseTarget)
	{

		CombatTarget = SeenPawn;
		ClearPatrolTimer();
		ChaseTarget();


		if (CombatDirector)
		{
			CombatDirector->RegisteredEnemies.AddUnique(this);
			CombatDirector->SelectAttacker();


			if (!GetWorldTimerManager().IsTimerActive(UpdateCombatTimer))
			{
				//GetWorld()->GetTimerManager().SetTimer(UpdateCombatTimer, this,
					//&AEnemy::UpdateCombatMovement, 0.3f,true);
			}

			//UpdateCombatMovement();
			
		}
		/*if (!Chased)
		{
			if (ICombatSoundInterface* CombatInterface = Cast<ICombatSoundInterface>(SeenPawn))
			{
				CombatInterface->EnemyStartChasing();
				Chased = true;
			}
		}*/

	}


	
}

void AEnemy::PawnHeard(APawn* SeenPawn,const FVector& Location, float Volume)
{ 

	/*
	if (CombatDirector)
	{
		CombatDirector->RegisteredEnemies.AddUnique(this);
	}
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

	}*/
}












