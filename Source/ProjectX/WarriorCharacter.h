// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Public\CharacterTypes.h"
#include "Public\Characters\BaseCharacter.h"
#include"Public\Interfaces\MotionWarpingInterface.h"
#include"Public\Interfaces\PickUpInterface.h"
#include "WarriorCharacter.generated.h"

class USpringArmComponent;
class AShield;
class UCameraComponent;
class AItemActor;
class ATreasure;
class USphereComponent;
class AEnemy;
class UCharacterHUD;
class AArenaGameMode;




UCLASS()
class PROJECTX_API AWarriorCharacter : public ABaseCharacter, public IPickUpInterface
{
	GENERATED_BODY()

public:

	AWarriorCharacter();

	/* <IHitInterface> */
	virtual void GetHit_Implementation(const FVector& ImpactPoint,AActor* Hitter) override;
	virtual void Jump() override;
	bool CheckShieldClose();
	bool CheckShieldOpen();
	void ClearShieldRegenerateTimer();
	void StartShieldRegenerateTimer();
	FORCEINLINE UAttributeComponent* GetAttributesComponent() const { return Attributes; }
	void Staminadeneme(float DeltaTime);
	/* </IHitInterface> */
	UPROPERTY(BlueprintReadOnly)
	int ComboCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
	UMaterialInterface* OverlayMaterial;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	bool IsEnemyBehindCharacter();
	void AddKilledEnemyID(FString EnemyName);


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SaveGame")
	TArray<FString>KilledEnemiesNames;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SaveGame")
	TArray<FString> AddedItems;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> WeaponClass;



protected:
	virtual void BeginPlay() override;
	void MoveForward(float value);
	void MoveRight(float value);
	//void Turn(float Value);
	//void LookUp(float Value);
	void EKeyPressed();
	/*
	* Combat
	*/
	void EquipWeapon(AWeapon*Weapon);
	virtual void Attack() override;
	virtual void AttackEnd() override;
	void Shield();
	void ShieldRealesed();
	void PlayEquipMontage(const FName& SectionName);	
	bool CanDisarm();
	virtual bool CanArm() override;
	virtual void HandleDamage(float DamageAmount) override;
   /********************************/
	bool ShieldAlive();
	void RegenerateShield();
	FTimerHandle shieldRegenerateTime;
	/********************************/
	
	FTimerHandle StaminaRegenerateTimer;
	void StaminaRegenerateTime();
	void StaminaClearTime();

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToBack();

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToHand();

	UFUNCTION(BlueprintCallable)
	void FinishEquipping();
	
	UFUNCTION(BlueprintCallable)
	void HitReactEnd();

	UFUNCTION(BlueprintCallable)
	void ComboCountReset();

	UFUNCTION(BlueprintCallable)
	virtual void DodgeEnd() override;

	

	UFUNCTION()
	void SphereCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor*OtherActor, UPrimitiveComponent*OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void SphereCollisionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(BlueprintReadOnly)
	TArray<AEnemy*> EnemiesInRange;
	
	UPROPERTY(BlueprintReadOnly)
	AEnemy* CloseEnemy;


	UPROPERTY(EditAnywhere, Category = "Settings")
	float DistanceThreshold ;

	bool BShieldOn = false;
	virtual void Die() override;


private:
	void Save();
	void LoadSaveGame();
	void SpawnDefaultWeapon();
	void DisArm();
	void Arm();
	void PlayShieldReactMontage();
	void InitializePlayerOverlay();
	void SetHealthBar();
	void SetStaminaBar();
	void SetLevelBar();
	void Dodge();
	bool IsUnoccupied();
	bool HasEnoughStamina();



	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;
	
	UPROPERTY(VisibleInstanceOnly)
	AItemActor* OverlappingItem;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AShield> ShieldClass;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* EquipMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* ShieldMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* ShieldReactMontage;

	UPROPERTY()
	AGameModeBase* GameMode;
	UPROPERTY()
	AArenaGameMode* ArenaGameMode;

	//////////////////////////////////
	/*ComboTimer*/
	UPROPERTY(EditAnywhere, Category = "Combat")
	float ComboResetTimer = 2.f;
	float TimeElapsed = 0.f;
	bool bAttackTimerOpen = false;
	///////////////////////////

	float StaminaResetTimerCount = 3.f;
	float StaminaTimeElapsed = 0.f;
	bool bStaminaTimerOpen = false;



	double CombatTargetRadius = 500.f;

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* Sphere;

	UPROPERTY(EditAnywhere)
	float CharacterRunSpeed = 600.f;

	UPROPERTY(EditAnywhere)
	float CharacterWalkSpeed = 250.f;
	void GetClosestEnemy();
	void SpawnDefaultShield();
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ECharacterStates CharacterStates = ECharacterStates::ECS_UnEquipped;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EMovementState MovementState = EMovementState::EMS_Idle;
	UPROPERTY()
	UCharacterHUD* PlayerOverlay;
public:
	virtual void Tick(float DeltaTime) override;
	void ComboCountTimer(float DeltaTime);
	void StaminaRegen();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void SetOverlappingItem(AItemActor* Item) override;  
	virtual void AddXp(AExperiencePoint* Xp) override;
	bool ExpGreaterMaxExp();
	void SetExpPoint(AExperiencePoint* Xp);
	virtual void AddGold(ATreasure* Treasure)override;
	virtual void AddHealth(AHealthPoint* Health)override;
	FORCEINLINE ECharacterStates GetCharacterStates() const { return CharacterStates; }
	FORCEINLINE EActionState GetActionState() const { return ActionState; }
	FORCEINLINE EMovementState GetMovementState() const { return MovementState; }

};
