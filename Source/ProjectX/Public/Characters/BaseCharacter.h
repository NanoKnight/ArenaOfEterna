// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include"./InterFaces/HitInterface.h"
#include"../EnumStates.h"
#include "NiagaraSystem.h"
#include "BaseCharacter.generated.h"


class AWeapon;
class ABaseItem;
class UAttributeComponent;
class UInventoryComponent;
class UAnimMontage;
class AEnemy;

UCLASS()
class PROJECTX_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();
  virtual void Tick(float DeltaTime) override;

  FORCEINLINE UInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }
  FORCEINLINE TEnumAsByte<EDeadPose> GetDeathPose() const { return DeadPose; }
 
  UPROPERTY(VisibleAnywhere,Category = Weapon)
  AWeapon* EquippedWeapon;

  UPROPERTY(EditAnywhere)
  TArray<ABaseItem*> ItemsToEquip;


  UPROPERTY(EditDefaultsOnly)
  USkeletalMeshComponent* EnemyOutlineMesh;




protected:
	virtual void BeginPlay() override;
	void InitializeEquipItems();
    virtual void Attack();
	virtual void GetHit_Implementation(const FVector& ImpactPoint,AActor* Hitter) override;
	void DirectionalHit(const FVector& ImpactPoint);
	virtual void HandleDamage(float DamageAmount);
	void PlayHitSound(const FVector& ImpactPoint);
	void PLayShieldHitSound(const FVector& ImpactPoint);
	void SpawnHitParticles(const FVector& ImpactPoint);
	void SpawnShieldHitParticles(const FVector& ImpactPoint);
	virtual void Die();
	virtual bool CanAttack();
	bool IsAlive();
	virtual void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);
  	void PlayHitReactMontage(const FName& SectionName);
    virtual int32 PlayAttackMontage();
	virtual int32 PlayHoldingAttackMontage();
	virtual int32 WarriorAttackMontage();
    virtual int32 PlayDeathMontage();	
	virtual void PlayDodgeMontage();
	void StopAttackMontage();
    void DisableCapsule();
	UFUNCTION(BlueprintCallable)
	FVector GetTranslationWarpTarget(AActor* Target);
	UFUNCTION(BlueprintCallable)
	FVector GetRotationWarpTarget(AActor* Target);
	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();
	virtual void DodgeEnd();
	virtual bool CanArm();
	void DisableCollision();

	
	bool RageMode = false;
   
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat)
	UAttributeComponent* Attributes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	UInventoryComponent* InventoryComponent;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = Combat)
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere,Category = Combat)
	double WarpTargetDistance = 75.f;
	
	UPROPERTY(VisibleAnywhere)
	int32 ComboCounts = 0;

	UPROPERTY(VisibleAnywhere)
	int32 HoldingComboCounts = 0;

	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EDeadPose>DeadPose;
private:

	virtual int32 PlayWarriorCountMontageSection(UAnimMontage* Montage,bool IsHolding,const TArray<FName>& SectionNames);
	void CheckComboCount(const int32& MaxSectionIndex, bool IsHolding);
	int32 PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames);
	UPROPERTY(EditAnywhere, Category = Sounds)
	USoundBase* HitSound;
	UPROPERTY(EditAnywhere, Category = Sounds)
	USoundBase* ShieldHitSound;


	UPROPERTY(EditAnywhere, Category = VFX)
	UParticleSystem* HitParticles;

	UPROPERTY(EditAnywhere, Category = VFX)
	UNiagaraSystem* HitNiagaraSystem;


	UPROPERTY(EditAnywhere, Category = VFX)
	UParticleSystem* ShieldParticles;
	

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = montages)
	UAnimMontage* HoldingAttackMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* DeathMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* DodgeMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
	TArray<FName> AttackMontageSections;

	UPROPERTY(EditAnywhere, Category = Combat)
	TArray<FName>HoldingAttackMontageSections;

	UPROPERTY(EditAnywhere, Category = Combat)
	TArray<FName> DeathMontageSections;

	
};
