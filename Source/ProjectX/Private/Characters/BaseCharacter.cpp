// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include "Components\BoxComponent.h"
#include"Components\CapsuleComponent.h"
#include"Items\Weapons\Weapon.h"
#include"Components/AttributeComponent.h"
#include"Components\InventorySystem\InventoryComponent.h"
#include"Kismet/KismetSystemLibrary.h"
#include"Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "CollisionQueryParams.h"
#include"CameraShakes\MainLegacyCameraShake.h"
#include "Camera/PlayerCameraManager.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"


// Sets default values
ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	EnemyOutlineMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("OutlineMesh"));
	EnemyOutlineMesh->SetupAttachment(GetMesh());
	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("EnemyAttributes"));
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	InitializeEquipItems();
}

void ABaseCharacter::InitializeEquipItems()
{
	int32 EquippedItemCount = InventoryComponent->EquippedItems.Num();
	for (int32 i = 0; i < EquippedItemCount; i++)

	{
		FInventoryStruct& EquipItems = InventoryComponent->EquippedItems[i];

		ABaseItem* SpawnedItem = GetWorld()->SpawnActor<ABaseItem>(EquipItems.ItemClass);


		if (EquipItems.ItemName.IsEmpty())
		{
			if (SpawnedItem)
			{
				SpawnedItem->Equip(GetMesh(), SpawnedItem->ItemSocketName, this, this);
				ItemsToEquip.Add(SpawnedItem);
				if (SpawnedItem->ItemType == EItemTypes::Weapon)
				{
					AWeapon* BaseWeapon = Cast<AWeapon>(SpawnedItem);
					EquippedWeapon = BaseWeapon;

				}
				else if (SpawnedItem->Defense > 0)
				{
					Attributes->SetDefense(Attributes->GetDefense() + SpawnedItem->Defense);
				}
			}
		}
		else
		{
			SpawnedItem->ItemName = EquipItems.ItemName;
			SpawnedItem->ItemIcon = EquipItems.ItemIcon;
			SpawnedItem->ItemEquipmentSlot = EquipItems.EquipmentSlot;
			SpawnedItem->ItemSocketName = EquipItems.ItemSocketName;
			SpawnedItem->ItemType = EquipItems.ItemTypes;
			SpawnedItem->GetItemMesh()->SetStaticMesh(EquipItems.ItemStaticMesh);

			if (SpawnedItem->ItemType == EItemTypes::Weapon)
			{
				AWeapon* EquipWeapon = Cast<AWeapon>(SpawnedItem);
				if (EquipWeapon)
				{
					EquippedWeapon = EquipWeapon;
				}
				else if (SpawnedItem->Defense > 0)
				{
					Attributes->SetDefense(Attributes->GetDefense() + SpawnedItem->Defense);
				}
			}
		}


	}
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::Attack()
{
	
}

void ABaseCharacter::GetHit_Implementation(const FVector& ImpactPoint,AActor* Hitter)
{
	if (IsAlive() && Hitter) {

		DirectionalHit(Hitter->GetActorLocation());
	
		if (Hitter->ActorHasTag("WarriorCharacter"))
		{
		
			APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
			if (PlayerController)
			{
				PlayerController->PlayerCameraManager->StartCameraShake(UMainLegacyCameraShake::StaticClass());
			}
		}
	}
	else
	{
		Die();

	}
	PlayHitSound(ImpactPoint);
	SpawnHitParticles(ImpactPoint);
}


	
	


void ABaseCharacter::Die()
{

	Tags.Add(FName("Dead"));
	PlayDeathMontage();

	
}

void ABaseCharacter::DisableCapsule()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

FVector ABaseCharacter::GetTranslationWarpTarget(AActor* Target)
{
	if(Target == nullptr) return FVector();

	const FVector CombatTargetLocation = Target->GetActorLocation();
	const FVector Location = GetActorLocation();

	 FVector TargetToMe = (Location - CombatTargetLocation).GetSafeNormal();
	 TargetToMe *= WarpTargetDistance;
	 return CombatTargetLocation + TargetToMe;
}

FVector ABaseCharacter::GetRotationWarpTarget(AActor* Target)
{
	if (Target)
	{
		 return Target->GetActorLocation();
		 
	}
	return FVector();
}

void ABaseCharacter::AttackEnd()
{
}

void ABaseCharacter::DodgeEnd()
{
}

bool ABaseCharacter::CanArm()
{
	return false;
}

void ABaseCharacter::DisableCollision()
{

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

bool ABaseCharacter::CanAttack()
{
	return false;
}

bool ABaseCharacter::IsAlive()
{
	return Attributes && Attributes->IsAlive();
}


int32 ABaseCharacter::PlayAttackMontage()
{
	
 return	PlayRandomMontageSection(AttackMontage,AttackMontageSections);
}

int32 ABaseCharacter::PlayHoldingAttackMontage()
{
	return PlayWarriorCountMontageSection(HoldingAttackMontage, true, HoldingAttackMontageSections);
}

int32 ABaseCharacter::WarriorAttackMontage()
{
	
	return PlayWarriorCountMontageSection(AttackMontage, false,AttackMontageSections);
	
	
}



int32 ABaseCharacter::PlayDeathMontage()
{
	const int32 Selection = PlayRandomMontageSection(DeathMontage, DeathMontageSections);
	TEnumAsByte<EDeadPose> Pose(Selection);
	if (Pose < EDeadPose::EDP_MAX)
	{
		DeadPose = Pose;
	}
	return Selection;

}

void ABaseCharacter::PlayDodgeMontage()
{
	if (DodgeMontage)
	{
		PlayMontageSection(DodgeMontage, FName("Default"));

	}
}

void ABaseCharacter::StopAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->Montage_Stop(0.25, AttackMontage);
	}
}

void ABaseCharacter::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);

		if (ActorHasTag(TEXT("Enemy")))
		{
			EnemyOutlineMesh->GetAnimInstance()->Montage_Play(HitReactMontage);
			EnemyOutlineMesh->GetAnimInstance()->Montage_JumpToSection(SectionName, HitReactMontage);
		}
	}
}

void ABaseCharacter::DirectionalHit(const FVector& ImpactPoint)
{
	const FVector Forward = GetActorForwardVector();
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

	const double CosTheta = FVector::DotProduct(Forward, ToHit);

	double Theta = FMath::Acos(CosTheta);

	Theta = FMath::RadiansToDegrees(Theta);

	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}

	FName Section("back");

	if (Theta >= -45.f && Theta < 45.f)
	{
		Section = FName("Front");
	}
	else if (Theta >= -135.f && Theta < -45.f)
	{
		Section = FName("Left");
	}
	else if (Theta >= 45.f && Theta < 135.f)
	{
		Section = FName("Right");

	}

	PlayHitReactMontage(Section);
	//UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + CrossProduct * 100, 5.f, FColor::Blue, 5.f);
	/*if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Cyan, FString::Printf(TEXT("Theta : %f"), Theta));
	}
	*/
	//UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + Forward * 60.f, 5.f, FColor::Red, 5.f);
	//UKismetSystemLibrary::DrawDebugArrow(this, GetActorLocation(), GetActorLocation() + ToHit * 60.f, 5.f, FColor::Green, 5.f);

}

void ABaseCharacter::PlayHitSound(const FVector& ImpactPoint)
{
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			ImpactPoint
		);
	}
}

void ABaseCharacter::PLayShieldHitSound(const FVector& ImpactPoint)
{
	if (ShieldHitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			ShieldHitSound,
			ImpactPoint
		);
	}
}

void ABaseCharacter::SpawnHitParticles(const FVector& ImpactPoint)
{

	if (HitParticles && GetWorld())
	{

		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitParticles,
			ImpactPoint
		);
	}
	FRotator Rotation = GetActorRotation();
	FVector Scale = FVector(1.0f);
	if (HitNiagaraSystem)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(

			GetWorld(),
			HitNiagaraSystem,
			ImpactPoint,
			Rotation,
			Scale,
			true,
			true,
			ENCPoolMethod::AutoRelease,
			true
			);
		
	}
}

void ABaseCharacter::SpawnShieldHitParticles(const FVector& ImpactPoint)
{
	if (ShieldParticles && GetWorld())
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			ShieldParticles,
			ImpactPoint
		);
	}
}

void ABaseCharacter::HandleDamage(float DamageAmount)
{

	if (Attributes )
	{
		Attributes->ReciveDamage(DamageAmount);
	}
}

void ABaseCharacter::PlayMontageSection(UAnimMontage* Montage, const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	   if (AnimInstance && Montage)
		{
			AnimInstance->Montage_Play(Montage);
			AnimInstance->Montage_JumpToSection(SectionName, Montage);
			if (ActorHasTag(TEXT("Enemy")))
			{
				EnemyOutlineMesh->GetAnimInstance()->Montage_Play(Montage);
				EnemyOutlineMesh->GetAnimInstance()->Montage_JumpToSection(SectionName, Montage);
			}
		}	
}

int32 ABaseCharacter::PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames)
{	
	if (SectionNames.Num() <= 0) return -1;
	int32 MaxSectionIndex = SectionNames.Num() - 1;
	int32 Selection = FMath::RandRange(0, MaxSectionIndex);
	PlayMontageSection(Montage, SectionNames[Selection]);
	return Selection;
}

int32 ABaseCharacter::PlayWarriorCountMontageSection(UAnimMontage* Montage,bool IsHolding, const TArray<FName>& SectionNames)
{
	if (SectionNames.Num() <= 0) return -1;
	int32 MaxSectionIndex = SectionNames.Num() - 1;
	int32 Selection = ComboCounts;
	int32 SelectionHolding = HoldingComboCounts;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	
	// if rage mode false play attack animation normal speed
	if (RageMode == false)
	{
		AnimInstance->Montage_Play(Montage, 1.f);
	}
	
	// if rage mode true play attack animation fast speed
	else if(RageMode == true)
	{
		AnimInstance->Montage_Play(Montage, 1.3f);
	}
	
	
	if (IsHolding)
	{
		AnimInstance->Montage_JumpToSection(SectionNames[SelectionHolding], Montage);

	}
	else
	{
		AnimInstance->Montage_JumpToSection(SectionNames[Selection], Montage);

	}
	CheckComboCount(MaxSectionIndex, IsHolding);
	return Selection;
}

void ABaseCharacter::CheckComboCount(const int32& MaxSectionIndex, bool IsHolding)
{
	if (ComboCounts >= HoldingAttackMontageSections.Num() - 1)
	{
		HoldingComboCounts = HoldingAttackMontageSections.Num()-2;

	}

	if (ComboCounts < MaxSectionIndex)
	{

		if (IsHolding == false)
		{
			ComboCounts = ComboCounts + 1;
			HoldingComboCounts = HoldingComboCounts + 1;

		}


	}

	else if (ComboCounts >= MaxSectionIndex)
	{
		ComboCounts = 0;
		HoldingComboCounts = 0;

	}
}



void ABaseCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{

	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		EquippedWeapon->IgnoreActors.Empty();
	}
}

