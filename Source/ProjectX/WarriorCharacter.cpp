// Fill out your copyright notice in the Description page of Project Settings.


#include "WarriorCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components\SphereComponent.h"
#include "Items\BaseItem.h"
#include "./HUD/ItemInteractionWidget.h"
#include"Items\Weapons\Weapon.h"
#include"Items\Weapons\Shield.h"
#include"Enemy\Enemy.h"
#include"Components/AttributeComponent.h"
#include "Animation/AnimMontage.h"
#include"Interfaces\HitInterface.h"
#include"HUD/PlayerHUD.h"
#include "HUD\CharacterHUD.h"
#include "SaveGames/EternaSaveGame.h"
#include "Items\ExperiencePoint.h"
#include"Items\Treasure.h"
#include"Items\HealthPoint.h"
#include"Items\EnemySpawner.h"
#include"Items\SpawnManager.h"
#include"HUD\InventoryWidget.h"
#include"Components\InventorySystem\InventoryComponent.h"
#include "Items\QuestActor.h"
#include "EngineUtils.h"
#include "UMG.h"
#include"Components\WidgetComponent.h"
#include "Public\QuestStruct.h"
#include"GameMode\ArenaGameMode.h"
#include"Runtime/Engine/Public/TimerManager.h"


// Sets default values
AWarriorCharacter::AWarriorCharacter()
{	
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block),
	GetMesh()->SetGenerateOverlapEvents(true);
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 300.f;
	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AWarriorCharacter::SphereCollisionBeginOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AWarriorCharacter::SphereCollisionEndOverlap);


}


void AWarriorCharacter::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add("WarriorCharacter");
	SpawnDefaultShield();
	SpawnDefaultWeapon();
	InitializePlayerOverlay();

	defaultCameraLoc = ViewCamera->GetRelativeLocation();


	FString Path = TEXT("/Script/Engine.DataTable'/Game/Blueprints/Data/DT_QuestDataTable.DT_QuestDataTable'");
	QuestDataTable = LoadObject<UDataTable>(nullptr, *Path);

	if (QuestDataTable)
	{
		TArray<FName> RowNames = QuestDataTable->GetRowNames();
		for (const FName& RowName : RowNames)
		{
			FQuestStruct* Quest = QuestDataTable->FindRow<FQuestStruct>(RowName, "");
			if (Quest)
			{
				ActiveQuests.Add(*Quest);
				CurrentQuest = ActiveQuests[0];
			}
		}

		if (ActiveQuests.IsValidIndex(0) && PlayerOverlay->GetQuestOverlay())
		{
			CurrentQuestIndex = 0;
			PlayerOverlay->GetQuestOverlay()->SetQuestText(
				ActiveQuests[0].QuestName,
				ActiveQuests[0].QuestDescription);
		}
		if (SpawnManager == nullptr)
		{
			TSubclassOf<ASpawnManager> SpawnManagerClass = ASpawnManager::StaticClass();
			SpawnManager = GetWorld()->SpawnActor<ASpawnManager>(SpawnManagerClass);
		}
	}	

	if (QuestActorClass)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			QuestActor = GetWorld()->SpawnActor<AQuestActor>(QuestActorClass, CurrentQuest.TargetLocation, FRotator::ZeroRotator);

		}
	}
}

void AWarriorCharacter::Save()
{
	
	GameMode = GetWorld()->GetAuthGameMode();
	ArenaGameMode = Cast<AArenaGameMode>(GameMode);
	if (ArenaGameMode) ArenaGameMode->SaveGame();
	
}

void AWarriorCharacter::LoadSaveGame()
{
	 GameMode = GetWorld()->GetAuthGameMode();
	ArenaGameMode = Cast<AArenaGameMode>(GameMode);
	if (ArenaGameMode)ArenaGameMode->LoadGame();	
}

void AWarriorCharacter::SpawnDefaultWeapon()
{
	
	UWorld* World = GetWorld();
	if (World && WeaponClass)
	{
		AWeapon* DefaultWeapon = World->SpawnActor<AWeapon>(WeaponClass);
		DefaultWeapon->Equip(GetMesh(), FName("Sword"), this, this);
		EquippedWeapon = DefaultWeapon;
		CharacterStates = ECharacterStates::ECS_EquippedOnehand;
		OverlappingItem = nullptr;
	}
}



void AWarriorCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	
	if (CheckShieldClose())
	{
		Super::GetHit_Implementation(ImpactPoint, Hitter);
		SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
		if (Attributes && Attributes->HealthPercent() > 0.f)
		{
			ActionState = EActionState::EAS_HitReaction;
		}

	}
	else if (CheckShieldOpen())
	{
		SpawnShieldHitParticles(ImpactPoint);
		PLayShieldHitSound(ImpactPoint);
		PlayShieldReactMontage();
		StartShieldRegenerateTimer(4);
		ClearShieldRegenerateTimer();

	}
	if (!ShieldAlive())
	{
		Super::GetHit_Implementation(ImpactPoint, Hitter);
		CharacterStates = ECharacterStates::ECS_EquippedOnehand;
		SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCharacterMovement()->MaxWalkSpeed = CharacterRunSpeed;
		ComboCountReset();
		StartShieldRegenerateTimer(2);
		ClearShieldRegenerateTimer();


	}
}


void AWarriorCharacter::Jump()
{
	if (IsUnoccupied())
	{
	Super::Jump();
	}

}

bool AWarriorCharacter::IsUnoccupied()
{
	return ActionState == EActionState::EAS_Unoccupied;
}

void AWarriorCharacter::Dodge()
{
	StaminaClearTime();
	StaminaRegenerateTime();
	

	if (ActionState != EActionState::EAS_Unoccupied || !HasEnoughStamina()) return;
		PlayDodgeMontage();
		ActionState = EActionState::EAS_Dodge;
		if (Attributes && PlayerOverlay)
	    {
		Attributes->ReciveStamina(Attributes->GetStaminaCost());
	    }
		
}

void AWarriorCharacter::CheckQuestProgress()
{
	if (CurrentQuest.QuestType == EQuestType::GoToLocation)
	{
		FVector PlayerLocation = GetActorLocation();
		float Distance = FVector::Dist(PlayerLocation, CurrentQuest.TargetLocation);
		if (Distance < 350.f)
		{
			CompleteCurrentQuest();
			QuestActor->HiddenQuestTracker(false);
		}

	}
	 if(CurrentQuest.QuestType == EQuestType::KillEnemies)
	{


		 FVector PlayerLocation = GetActorLocation();
		 float Distance = FVector::Dist(PlayerLocation, CurrentQuest.TargetLocation);

		 if (QuestActor && Distance < 350.f)
		 {
			QuestActor->HiddenQuestTracker(false);
		 }




		if (CurrentQuest.CurrentKillCount >= CurrentQuest.TargetKillCount)
		{


			if (SpawnManager)
			{
				AEnemySpawner* NextSpawner = SpawnManager->GetNextSpawn();
				if (NextSpawner)
				{
					NextSpawner->SpawnEnemy(NextSpawner->EnemySpawnCount);
				}

			}
			
			
			CompleteCurrentQuest();
			
		}


	}

}

void AWarriorCharacter::StartNextQuest()
{
	if (QuestDataTable)
	{
		

		static const FString ContextString(TEXT("Quest"));
		FQuestStruct* NextQuest = QuestDataTable->FindRow<FQuestStruct>(NextQuestRowName, ContextString);
		if (NextQuest)
		{
			CurrentQuest = *NextQuest;
			if (PlayerOverlay)
			{
				PlayerOverlay->GetQuestOverlay()->SetQuestText(CurrentQuest.QuestName, CurrentQuest.QuestDescription);
			}
		}

	}
}

void AWarriorCharacter::QuesstCompleteFadeOutAnim()
{
	if (PlayerOverlay)
	{
		PlayerOverlay->PlayAnimation(PlayerOverlay->QuestCompleteFadeOut);
	}
}



bool AWarriorCharacter::HasEnoughStamina()
{
	return Attributes->GetStamina() > Attributes->GetStaminaCost();
}



bool AWarriorCharacter::CheckShieldClose()
{
	return !BShieldOn || IsEnemyBehindCharacter();
}

bool AWarriorCharacter::CheckShieldOpen()
{
	return BShieldOn || !IsEnemyBehindCharacter() || ShieldAlive();
}

void AWarriorCharacter::RegenerateShield()
{
	Attributes->RegenerateShield();
}

void AWarriorCharacter::ClearShieldRegenerateTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(Attributes->Timer);
}

void AWarriorCharacter::StartShieldRegenerateTimer(float Time)
{
	GetWorld()->GetTimerManager().SetTimer(shieldRegenerateTime, this, &AWarriorCharacter::RegenerateShield, Time, false);

}

void AWarriorCharacter::StaminaRegenerate(float DeltaTime)
{
	if (Attributes && PlayerOverlay)
	{
		Attributes->RegenStamina(DeltaTime);
	}
}

void AWarriorCharacter::GetClosestEnemy()
{
    AEnemy* NewClosestEnemy = nullptr;
	float MinDistance = FLT_MAX;
	for (AEnemy* Enemy : EnemiesInRange)
	{
		float Distance = FVector::Dist(this->GetActorLocation(), Enemy->GetActorLocation());
		if (Distance < MinDistance)
		{
			MinDistance = Distance;
			NewClosestEnemy = Enemy;
		}			
	}
	if (CloseEnemy != NewClosestEnemy)
	{
		if (CloseEnemy) {

			USkeletalMeshComponent* EnemyMesh = CloseEnemy->EnemyOutlineMesh;
			if (EnemyMesh)
			{
				EnemyMesh->SetMaterial(0, DefaultMat);
			}
				
		}
		if (NewClosestEnemy && NewClosestEnemy->EnemyState != EEnemyState::EAS_Stun)
		{
			USkeletalMeshComponent* EnemyMesh = NewClosestEnemy->EnemyOutlineMesh;
			if (EnemyMesh)
			{
				EnemyMesh->SetMaterial(0, OverlayMaterial);
				

			}
		}
	}
	CloseEnemy = NewClosestEnemy;
	
	}

void AWarriorCharacter::SpawnDefaultShield()
{
	UWorld* World = GetWorld();
	if (World && ShieldClass)
	{
		AShield* Shield = World->SpawnActor<AShield>(ShieldClass) ;
		Shield->Equip(GetMesh(), FName("Shield"),this,this);

	}
}

float AWarriorCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{

   if (!IsEnemyBehindCharacter())
	{
		if (BShieldOn)
		{
			
			HandleDamage(DamageAmount);
		}
	}

	if (!BShieldOn)
	{
		HandleDamage(DamageAmount);
		SetHealthBar();
	}
	return DamageAmount;
}
 
void AWarriorCharacter::SetHealthBar()
{
	if (PlayerOverlay && Attributes)
	{
		PlayerOverlay->SetHealthBarPercent(Attributes->HealthPercent());
	}
}

void AWarriorCharacter::SetStaminaBar()
{
	if (PlayerOverlay && Attributes)
	{
		PlayerOverlay->SetStaminaBarPercent(Attributes->StaminaPercent());
	}
}

void AWarriorCharacter::SetLevelBar()
{
	if (PlayerOverlay && Attributes)
	{
		PlayerOverlay->SetLevelBarPercent(Attributes->LevelBarPercent());
	}
}

void AWarriorCharacter::PrintQuest()
{
	for (const FQuestStruct& Quest: ActiveQuests)
	{

	}
}

bool AWarriorCharacter::IsEnemyBehindCharacter()
{

   FVector WarriorCharacterLocation = GetActorLocation();
   FVector EnemyLocation = CloseEnemy->GetActorLocation();
   FVector WarriorForwardVector = GetActorForwardVector();
   FVector DirectionToEnemy = EnemyLocation - WarriorCharacterLocation;
   DirectionToEnemy.Normalize();
   float DotProduct = FVector::DotProduct(WarriorForwardVector, DirectionToEnemy);
   return DotProduct < 0;
	
}

void AWarriorCharacter::AddKilledEnemyID(FString EnemName)
{
	if (KilledEnemiesNames.Contains(EnemName))
	{
		KilledEnemiesNames.Add(EnemName);
	}

}

void AWarriorCharacter::AddQuest(const FQuestStruct& NewQuest)
{
	ActiveQuests.Add(NewQuest);  
}


void AWarriorCharacter::UpdateQuest(FName QuestRowName)
{
	if (QuestDataTable)
	{
		static const FString ContextString(TEXT("Quest Lookup"));
		FQuestStruct* Quest = QuestDataTable->FindRow<FQuestStruct>(QuestRowName, ContextString);

		if (Quest)
		{
			CurrentQuest = *Quest;
			CurrentQuestRowName = QuestRowName;
		}
	}

}


void AWarriorCharacter::InitializePlayerOverlay()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		APlayerHUD* PlayerHUD = Cast<APlayerHUD>(PlayerController->GetHUD());

		if (PlayerHUD)
		{
			PlayerOverlay = PlayerHUD->GetPlayerOverlay();
			if (PlayerOverlay && Attributes)
			{
				PlayerOverlay->SetStaminaBarPercent(Attributes->StaminaPercent());
				PlayerOverlay->SetHealthBarPercent(Attributes->HealthPercent());
				PlayerOverlay->SetLevelBarPercent(Attributes->LevelBarPercent());
				PlayerOverlay->SetGoldText(Attributes->GetGold());
				PlayerOverlay->SetXpText(Attributes->GetExperience());
				PlayerOverlay->SetLevelText(Attributes->GetLevel());
				PlayerOverlay->SetMaxXpText(Attributes->GetMaxExperience());

			}
		}
	}
}


void AWarriorCharacter::MoveForward(float value)
{
	if (ActionState != EActionState::EAS_Unoccupied) return;
	if(Controller && (value != 0.f))

	{
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, value);
		bForward = true;
		if (bForward == true)
		{
			MovementState = EMovementState::EMS_Run;

		}
		
	}

	else
	{
		MovementState = EMovementState::EMS_Idle;
		bForward = false;
	}
}



void AWarriorCharacter::MoveRight(float value)
{
	if (ActionState != EActionState::EAS_Unoccupied) return;
	if (Controller && (value != 0.f))
	{
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, value);
	   MovementState = EMovementState::EMS_Run;
	}
	else
	{
		if (bForward == false)
		{
			MovementState = EMovementState::EMS_Idle;

		}
	}

}


void AWarriorCharacter::CameraForward(float Value)
{
	if (Controller && (Value != 0.f) && bCanMoveCamera)
	{ 
		FVector NewLocation = ViewCamera->GetRelativeLocation();
		NewLocation.X += Value * CameraMoveSpeed;
		NewLocation.X = FMath::Clamp(NewLocation.X, MinX, MaxX);
		ViewCamera->SetRelativeLocation(NewLocation);
		
	}
}

void AWarriorCharacter::CameraRight(float Value)
{
	if (Controller && (Value != 0.f) && bCanMoveCamera)
	{
		FVector NewLocation = ViewCamera->GetRelativeLocation();
		NewLocation.Y += Value * CameraMoveSpeed;
		NewLocation.Y = FMath::Clamp(NewLocation.Y, MinY, MaxY);
		ViewCamera->SetRelativeLocation(NewLocation);
		
	}
}

/*void AWarriorCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}
*/
void AWarriorCharacter::EKeyPressed()
{

	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	if (OverlappingWeapon)
	{
		EquipWeapon(OverlappingWeapon);
	}

	else {
		if (CanDisarm())
		{
			DisArm();

		}
		else if (CanArm())
		{
			Arm();
		}

	}
}

void AWarriorCharacter::Interact()
{
	ABaseItem* OverlappingWeapon = Cast<ABaseItem>(OverlappingItem);
	if (OverlappingWeapon)
	{
		OverlappingWeapon->PickUp(this);
	}
}

void AWarriorCharacter::OpenInventory()
{
	InventoryComponent->OpenInventory();

}

void AWarriorCharacter::EquipItem(const FInventoryStruct& Item)
{
	if (InventoryComponent)
	{
		InventoryComponent->EquipItem(Item);

	}
}

void AWarriorCharacter::MoveCamera()
{
	bCanMoveCamera = true;
}

void AWarriorCharacter::MoveCameraReleased()
{
	bCanMoveCamera = false;

}


void AWarriorCharacter::Die()
{
	Super::Die();
	ActionState = EActionState::EAS_Dead;

	if (DeathWidgetClass)
	{
		DeathWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), DeathWidgetClass);
		if (DeathWidgetInstance)
		{
			DeathWidgetInstance->AddToViewport();

			APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

			if (PlayerController)
			{
				
				PlayerController->bShowMouseCursor = true;

				FInputModeUIOnly InputMode;
				InputMode.SetWidgetToFocus(DeathWidgetInstance->TakeWidget());
				PlayerController->SetInputMode(InputMode);
			}

		}
	}
	

}

void AWarriorCharacter::DisArm()
{
	PlayEquipMontage(FName("Unequip"));
	CharacterStates = ECharacterStates::ECS_UnEquipped;
	ActionState = EActionState::EAS_EquippingWeapon;
}

void AWarriorCharacter::Arm()
{
	PlayEquipMontage(FName("Equip"));
	CharacterStates = ECharacterStates::ECS_EquippedOnehand;
	ActionState = EActionState::EAS_EquippingWeapon;
}

void AWarriorCharacter::PlayShieldReactMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (ShieldReactMontage && AnimInstance)
	{
		AnimInstance->Montage_Play(ShieldReactMontage);
	}
}

void AWarriorCharacter::EquipWeapon(AWeapon* Weapon)
{
	//Weapon->Equip(GetMesh(), FName("Sword"), this, this);
	CharacterStates = ECharacterStates::ECS_EquippedOnehand;
	OverlappingItem = nullptr;
	EquippedWeapon = Weapon;
}

void AWarriorCharacter::Attack()
{
		Super::Attack();
		bDidHoldingAttack = false;
		AttackButtonStates = EAttackButtonState::EAB_Holding;
		GetWorld()->GetTimerManager().SetTimer(AttackHoldingTimer, this, &AWarriorCharacter::PlayHoldingAttackAnim, 1.f, false);

		
}

void AWarriorCharacter::AttackReleassed()
{
	if (bDidHoldingAttack)
	{
		return;
	}

	AttackButtonStates = EAttackButtonState::EAB_Releassed;
	const bool bCanAttack = (ActionState == EActionState::EAS_Unoccupied && CharacterStates != ECharacterStates::ECS_UnEquipped);
	if (bCanAttack)
	{
		WarriorAttackMontage();
		bAttackTimerOpen = true;
		ActionState = EActionState::EAS_Attacking;
	}
	

}


void AWarriorCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_Unoccupied;

}

void AWarriorCharacter::UsingSkill()
{
	ActionState = EActionState::EAS_UsingSkill;
}


void AWarriorCharacter::FirstSkill()
{
	if (ActionState == EActionState::EAS_UsingSkill) return;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		UsingSkill();
		AnimInstance->Montage_Play(FirstSkillMontage);
		
	}

}

void AWarriorCharacter::SecondSkill()
{
	if (ActionState == EActionState::EAS_UsingSkill) return;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	RageMode = true;
	UsingSkill();
	if (AnimInstance)
	{
		AnimInstance->Montage_Play(SecondSkillMontage);
	}
	DefaultEquippedWeaponDamage = EquippedWeapon->GetDamage();
	EquippedWeapon->SetDamage(EquippedWeapon->GetDamage() * 5);
	GetCharacterMovement()->MaxWalkSpeed = 800.f;
	GetWorld()->GetTimerManager().SetTimer(SecondSkillTimer, this, &AWarriorCharacter::DefaultVar, Ragetime, false);

}


void AWarriorCharacter::SkillCanDamageF(float SphereRadiusFloat, float SkillDamageFloat, float TraceEnd)
{
	TArray<FHitResult> OutHits;
	FVector Start = GetActorLocation();
	FVector End = Start + GetActorForwardVector() * TraceEnd;
	float SphereRadius = SphereRadiusFloat;
	float SkillDamage = SkillDamageFloat;
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this);
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(this);
	IgnoredActors.Add(EquippedWeapon);
	ECollisionChannel TraceChannel = ECC_WorldDynamic;
	ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(TraceChannel);

	bool bHit = UKismetSystemLibrary::SphereTraceMulti(
		GetWorld(),
		Start,
		End,
		SphereRadius,
		TraceType,
		false,
		IgnoredActors,
		EDrawDebugTrace::None,
		OutHits,
		true
	);

	TSet<AActor*> DamagedEnemies;

	if (bHit)
	{
		for (auto& hit : OutHits)
		{
			AActor* HitActor = hit.GetActor();
			if (HitActor && HitActor->IsA(AEnemy::StaticClass()))
			{
				
				if (!DamagedEnemies.Contains(HitActor))
				{
					DamagedEnemies.Add(HitActor);
					AEnemy* Enemy = Cast<AEnemy>(hit.GetActor());
					if (!Enemy->IsDead())
					{
						Enemy->SetRagdoll();
						UGameplayStatics::ApplyDamage(HitActor, SkillDamage, GetInstigator()->GetController(), this, UDamageType::StaticClass());
						GetSkillHit(hit);
					}
					
				}
			}
		}
	}
}

void AWarriorCharacter::CompleteCurrentQuest()
{

	if (!CurrentQuest.QuestName.IsEmpty()) 
	{
		
		FString CurrentRowName = NextQuestRowName.ToString();
		FString BaseName = "Quest";
		int32 QuestNumber = 2;
		if (CurrentRowName.Split(TEXT("_"),&BaseName, &CurrentRowName))
		{
			QuestNumber = FCString::Atoi(*CurrentRowName);
			QuestNumber++;
		}
		NextQuestRowName = FName(FString::Printf(TEXT("%s_%d"), *BaseName, QuestNumber));
		StartNextQuest();
		if (PlayerOverlay->GetQuestCompleteWidget())
		{
		PlayerOverlay->GetQuestCompleteWidget()->SetQuestText(CurrentQuest.QuestName);
		PlayerOverlay->GetQuestCompleteWidget()->PlayFadeInAnimation();

		PlayerOverlay->PlayAnimation(PlayerOverlay->QuestCompleteFadeIn);
		GetWorld()->GetTimerManager().SetTimer(QuestCompleteUITimer, this, &AWarriorCharacter::QuesstCompleteFadeOutAnim, 3, false);		

		}
	}
	if (QuestActorClass && !QuestActor)
	{
		QuestActor = GetWorld()->SpawnActor<AQuestActor>(QuestActorClass, CurrentQuest.TargetLocation, FRotator::ZeroRotator);
	
	
	}
	else
	{
		QuestActor->SetActorLocation(CurrentQuest.TargetLocation);
		QuestActor->HiddenQuestTracker(true);

	}

}
	
void AWarriorCharacter::SkillEnd()
{


	ActionState = EActionState::EAS_Unoccupied;

}


void AWarriorCharacter::Shield()
{
	
	if (ActionState == EActionState::EAS_UsingSkill)return;
	
	if (CharacterStates == ECharacterStates::ECS_UnEquipped)return;

	for (int32 i = 0; i < GetInventoryComponent()->EquippedItems.Num(); i++)
	{
		FInventoryStruct& Item = GetInventoryComponent()->EquippedItems[i];
		if (Item.ItemTypes == EItemTypes::Shield)
		{
			if (CloseEnemy)
			{
				FString EnemyName = CloseEnemy->GetName();
				
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Enemy : %s"), *EnemyName));
				FVector EnemyLocation = CloseEnemy->GetActorLocation();
				FVector CharcterLOCation = GetActorLocation();

				FRotator NewRot = (EnemyLocation - CharcterLOCation).Rotation();
				SetActorRotation(NewRot);
			
			
			
			
			}
			if (ShieldAlive() && ActionState != EActionState::EAS_Dead && ActionState != EActionState::EAS_UsingSkill)
			{
				UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
				if (AnimInstance && ShieldMontage)
				{
					AnimInstance->Montage_Play(ShieldMontage);
				}
				BShieldOn = true;
				CharacterStates = ECharacterStates::ECS_EquippedShield;
				ActionState = EActionState::EAS_Unoccupied;
				GetCharacterMovement()->MaxWalkSpeed = CharacterWalkSpeed;
			}
		}
	}


	
}

void AWarriorCharacter::ShieldRealesed()
{
	if (CharacterStates == ECharacterStates::ECS_UnEquipped) return;
	CharacterStates = ECharacterStates::ECS_EquippedOnehand;
	BShieldOn = false;
	GetCharacterMovement()->MaxWalkSpeed = CharacterRunSpeed;
	StartShieldRegenerateTimer(2);



	
}

void AWarriorCharacter::PlayEquipMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}

bool AWarriorCharacter::CanDisarm()
{
	return ActionState == EActionState::EAS_Unoccupied &&
		CharacterStates != ECharacterStates::ECS_UnEquipped;
		
}

bool AWarriorCharacter::CanArm()
{
	return ActionState == EActionState::EAS_Unoccupied&&
		CharacterStates == ECharacterStates::ECS_UnEquipped&&
	    EquippedWeapon;
}

void AWarriorCharacter::HandleDamage(float DamageAmount)
{
	if (Attributes && BShieldOn)
	{
		Attributes->ReciveShieldDamage(DamageAmount);
	}
	
    if (Attributes && !BShieldOn)
	{
		Attributes->ReciveDamage(DamageAmount);
	}
}



bool AWarriorCharacter::ShieldAlive()
{
	return Attributes && Attributes->IsShieldAlive();
}

void AWarriorCharacter::DefaultVar()
{
	EquippedWeapon->SetDamage(DefaultEquippedWeaponDamage);
	GetCharacterMovement()->MaxWalkSpeed = CharacterRunSpeed;
	RageMode = false;
}

void AWarriorCharacter::ChangeAttackType()
{
	AttackButtonStates = EAttackButtonState::EAB_Holding;
}

void AWarriorCharacter::PlayHoldingAttackAnim()
{
	if (AttackButtonStates == EAttackButtonState::EAB_Holding)
	{
		PlayHoldingAttackMontage();
		bDidHoldingAttack = true;
		bAttackTimerOpen = true;
		
	}	
}

void AWarriorCharacter::StaminaRegenerateTime()
{
	GetWorld()->GetTimerManager().SetTimer(StaminaRegenerateTimer, this, &AWarriorCharacter::StaminaRegen, 0.6, false);
	
}

void AWarriorCharacter::StaminaClearTime()
{
	GetWorld()->GetTimerManager().ClearTimer(Attributes->StaminaTimers);
}

void AWarriorCharacter::AttachWeaponToBack()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("BackSword"));
	}
}

void AWarriorCharacter::AttachWeaponToHand()
{
	if (EquippedWeapon) 
	{
		EquippedWeapon->AttachMeshToSocket(GetMesh(), FName("Sword"));
		ActionState = EActionState::EAS_EquippingWeapon;
	
	}
}
void AWarriorCharacter::FinishEquipping()
{
	ActionState = EActionState::EAS_Unoccupied;
}
void AWarriorCharacter::HitReactEnd()
{
	ActionState = EActionState::EAS_Unoccupied;
}
void AWarriorCharacter::ComboCountReset()
{
	ComboCounts = 0;
	HoldingComboCounts = 0;
	bAttackTimerOpen = false;

}

void AWarriorCharacter::DodgeEnd()
{
	ActionState = EActionState::EAS_Unoccupied;

}

void AWarriorCharacter::SphereCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && (OtherActor != this)&& OtherComp)
	{
		AEnemy* Enemy = Cast<AEnemy>(OtherActor);
		if (Enemy)
		{		
			if (Enemy->EnemyState != EEnemyState::EES_Dead)
			{
				EnemiesInRange.Add(Enemy);				
			}
			else
			{
				EnemiesInRange.Remove(Enemy);
			}		
		}

		ABaseItem* ItemRef = Cast<ABaseItem>(OtherActor);
		if (ItemRef)
		{
			ItemRef->SetInteractionVisibility(true);

                ///      BU KISIM OPTÝMÝZE EDÝLMELÝ  
			UWidgetComponent* WidgetComp = ItemRef->GetItemInteractionWidget();
			if (WidgetComp)
			{
				UUserWidget* UserWidget = WidgetComp->GetUserWidgetObject();
				if (UserWidget)
				{
					UItemInteractionWidget* ItemInteractWidget = Cast<UItemInteractionWidget>(UserWidget);
					if (ItemInteractWidget)
					{
						if (ItemRef->ItemType == EItemTypes::Weapon)
						{
							ItemInteractWidget->SetInteractionValues(ItemRef->ItemName, ItemRef->Damage,ItemRef->ItemType);

						}
						else
						{
							ItemInteractWidget->SetInteractionValues(ItemRef->ItemName, ItemRef->Defense,ItemRef->ItemType);

						}

					}
				}

				
			}

			
		}
		
		
		
	}	
}

void AWarriorCharacter::SphereCollisionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	if (OtherActor && (OtherActor != this)&& OtherComp)
	{

		AEnemy* Enemy = Cast<AEnemy>(OtherActor);
		if (Enemy)
		{
			EnemiesInRange.Remove(Enemy);
		}


	
			ABaseItem* ItemRef = Cast<ABaseItem>(OtherActor);
			if (ItemRef)
			{
				ItemRef->SetInteractionVisibility(false);
			}
		
		


	}

}

void AWarriorCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &AWarriorCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &AWarriorCharacter::MoveRight);
	//PlayerInputComponent->BindAxis(FName("Turn"), this, & AWarriorCharacter::Turn);
    //PlayerInputComponent->BindAxis(FName("LookUp"), this, &AWarriorCharacter::LookUp);
	PlayerInputComponent->BindAxis(FName("CameraForward"), this, &AWarriorCharacter::CameraForward);
	PlayerInputComponent->BindAxis(FName("CameraRight"), this, &AWarriorCharacter::CameraRight);
	PlayerInputComponent->BindAction(FName("MoveCamera"), IE_Pressed, this, &AWarriorCharacter::MoveCamera);
	PlayerInputComponent->BindAction(FName("MoveCamera"), IE_Released, this, &AWarriorCharacter::MoveCameraReleased);
	PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(FName("Use"), IE_Pressed, this, &AWarriorCharacter::Interact);
	PlayerInputComponent->BindAction(FName("Inventory"), IE_Pressed, this, &AWarriorCharacter::OpenInventory);
	PlayerInputComponent->BindAction(FName("Attack"), IE_Pressed, this, &AWarriorCharacter::Attack);
	PlayerInputComponent->BindAction(FName("Attack"), IE_Released, this, &AWarriorCharacter::AttackReleassed);
	PlayerInputComponent->BindAction(FName("Shield"), IE_Pressed, this, &AWarriorCharacter::Shield);
	PlayerInputComponent->BindAction(FName("Shield"), IE_Released, this, &AWarriorCharacter::ShieldRealesed);
	PlayerInputComponent->BindAction(FName("Dodge"), IE_Pressed, this, &AWarriorCharacter::Dodge);
	PlayerInputComponent->BindAction(FName("SaveGame"), IE_Pressed, this, &AWarriorCharacter::Save);
	PlayerInputComponent->BindAction(FName("FirstSkill"), IE_Pressed, this, &AWarriorCharacter::FirstSkill);
	PlayerInputComponent->BindAction(FName("SecondSkill"), IE_Pressed, this, &AWarriorCharacter::SecondSkill);
	PlayerInputComponent->BindAction(FName("CompleteQuest"), IE_Pressed, this, &AWarriorCharacter::CompleteCurrentQuest);


}


void AWarriorCharacter::SetOverlappingItem(ABaseItem* Item)
{
	OverlappingItem = Item;
}

void AWarriorCharacter::AddXp(AExperiencePoint* Xp)
{
	if (Attributes && PlayerOverlay)
	{
		SetExpPoint(Xp);

		if (ExpGreaterMaxExp())
		{
			Attributes->LevelUp();
			PlayerOverlay->SetXpText(Attributes->GetExperience());
			PlayerOverlay->SetMaxXpText(Attributes->GetMaxExperience());
			PlayerOverlay->SetLevelText(Attributes->GetLevel());
		}
		SetLevelBar();
	}
	
}
bool AWarriorCharacter::ExpGreaterMaxExp()
{
	return Attributes->GetExperience() >= Attributes->GetMaxExperience();
}
void AWarriorCharacter::SetExpPoint(AExperiencePoint* Xp)
{

	Attributes->AddExperience(Xp->GetExperience());
	PlayerOverlay->SetXpText(Attributes->GetExperience());
	PlayerOverlay->SetMaxXpText(Attributes->GetMaxExperience());

}
void AWarriorCharacter::AddGold(ATreasure* Treasure)
{	
	if (Attributes && PlayerOverlay)
	{
		Attributes->AddGold(Treasure->GetGold());
		PlayerOverlay->SetGoldText(Attributes->GetGold());
	}
}
void AWarriorCharacter::AddHealth(AHealthPoint* Health)
{
	if (Attributes && PlayerOverlay)
	{
		Attributes->AddHealth(Health->GetHealth());
		SetHealthBar();
	}

}
void AWarriorCharacter::ExecuteGetHit(FHitResult& BoxHit)
{
	IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());

	if (HitInterface)
	{

		HitInterface->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint, GetOwner());
		

	}
	IgnoreActors.AddUnique(BoxHit.GetActor());
}

void AWarriorCharacter::GetSkillHit(FHitResult& Skillhit)
{

	ISkillHitInterface* SkillInterface = Cast<ISkillHitInterface>(Skillhit.GetActor());

	if (SkillInterface)
	{
		SkillInterface->SkillHit(Skillhit.ImpactPoint, GetOwner());
	}
}

void AWarriorCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GetClosestEnemy();
	ComboCountTimer(DeltaTime);
	SetStaminaBar();
	ResetCameraPosition();
	CheckQuestProgress();


	if (BShieldOn)
	{
		if (CloseEnemy)
		{
			FString EnemyName = CloseEnemy->GetName();
			FVector EnemyLocation = CloseEnemy->GetActorLocation();
			FVector CharcterLOCation = GetActorLocation();
			FRotator NewRot = (EnemyLocation - CharcterLOCation).Rotation();
			SetActorRotation(NewRot);
		}
	}


	


}
void AWarriorCharacter::ResetCameraPosition()
{
	if (!bCanMoveCamera)
	{
		FVector Currentval = ViewCamera->GetRelativeLocation();
		FVector Newloc = FMath::VInterpTo(Currentval, defaultCameraLoc, GetWorld()->DeltaTimeSeconds, 7.f);
		ViewCamera->SetRelativeLocation(Newloc);
	}
}
void AWarriorCharacter::ComboCountTimer(float DeltaTime)
{
	if (bAttackTimerOpen)
	{

		if (ActionState != EActionState::EAS_Attacking)
		{
			TimeElapsed += DeltaTime;
		}
		if (TimeElapsed >= ComboResetTimer)
		{

			ComboCountReset();
			TimeElapsed = 0;

		}
	}

	
}
void AWarriorCharacter::StaminaRegen()
{
	if (Attributes)
	{
		Attributes->RegenerateStamina();
	}	

}





