// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include"./InterFaces/HitInterface.h"
#include"../CharacterTypes.h"
#include "NiagaraSystem.h"
#include "BaseCharacter.generated.h"


class AWeapon;

class UAttributeComponent;
class UAnimMontage;
class AEnemy;

UCLASS()
class PROJECTX_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();
  virtual void Tick(float DeltaTime) override;
  FORCEINLINE TEnumAsByte<EDeadPose> GetDeathPose() const { return DeadPose; }
 
  UPROPERTY(VisibleAnywhere, Category = Weapon)
  AWeapon* EquippedWeapon;

  UPROPERTY(EditDefaultsOnly)
  USkeletalMeshComponent* EnemyOutlineMesh;



protected:
	virtual void BeginPlay() override;
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

   
	UPROPERTY(EditAnywhere, Category = Combat)
	UAttributeComponent* Attributes;

	
	UPROPERTY(BlueprintReadOnly, Category = Combat)
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere,Category = Combat)
	double WarpTargetDistance = 75.f;
	
	UPROPERTY(VisibleAnywhere)
	int32 ComboCounts = 0;

	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EDeadPose>DeadPose;
private:

	 int32 PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames);
	 int32 PlayWarriorCountMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames);
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

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* DeathMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* DodgeMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
	TArray<FName> AttackMontageSections;

	UPROPERTY(EditAnywhere, Category = Combat)
	TArray<FName> DeathMontageSections;

	
};
