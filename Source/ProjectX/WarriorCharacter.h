// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Public\CharacterTypes.h"
#include "Public\Characters\BaseCharacter.h"
#include"Blueprint/UserWidget.h"
#include"Public\Interfaces\PickUpInterface.h"
#include "Public\QuestStruct.h"
#include "WarriorCharacter.generated.h"


class USpringArmComponent;
class AShield;
class UCameraComponent;
class AItemActor;
class ATreasure;
class USphereComponent;
class AEnemy;
class UCharacterHUD;
class UQuestUI;
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

	UFUNCTION(BlueprintCallable, Category = "Quests")
	void StartQuest(FName QuestRowName);
	UFUNCTION(BlueprintCallable, Category = "Quests")
	void AddQuest(const FQuestStruct& NewQuest);

	UFUNCTION(BlueprintCallable, Category = "Quests")
	void CompleteQuest(int32 QuestID);

	UFUNCTION(BlueprintCallable, Category = "Quests")
	void UpdateQuestUI();

	void UpdateQuest(FName QuestRowName);
	FQuestStruct GetCurrentQuest() const { return CurrentQuest; }







	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SaveGame")
	TArray<FString>KilledEnemiesNames;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SaveGame")
	TArray<FString> AddedItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quests")
	TArray<FQuestStruct> ActiveQuests;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UQuestUI> QuestWidget;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> WeaponClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> DeathWidgetClass;
	UUserWidget* DeathWidgetInstance;

	UPROPERTY(EditAnywhere,BlueprintReadWrite , Category= "Quests")
	UDataTable* QuestDataTable;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest")
	FQuestStruct CurrentQuest;

	UPROPERTY()
	int32 CurrentQuestIndex;

	FName CurrentQuestRowName;
  
    


protected:
	virtual void BeginPlay() override;
	void MoveForward(float value);
	void MoveRight(float value);
	//void Turn(float Value);
	//void LookUp(float Value);
	void CameraForward(float Value);
	void CameraRight(float Value);
	void EKeyPressed();
	void MoveCamera();
	void MoveCameraReleased();
	/*
	* Combat
	*/
	void EquipWeapon(AWeapon*Weapon);
	virtual void Attack() override;
	virtual void AttackEnd() override;
	void UsingSkill();
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
	FTimerHandle SecondSkillTimer;

	void DefaultVar();

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

	UFUNCTION(BlueprintCallable)
	void SkillEnd();


	UFUNCTION(BlueprintCallable)
	void SkillCanDamageF();
	
	UFUNCTION()
	void CompleteCurrentQuest();

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float CameraMoveSpeed = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float MinX = -1.f; // Minimum X sýnýrý

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float MaxX = 400.f;  // Maximum X sýnýrý

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float MinY = -300.f; // Minimum Y sýnýrý

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float MaxY = 300.f;  // Maximum Y sýnýrý


private:
	void Save();
	void LoadSaveGame();
	void SpawnDefaultWeapon();
	void DisArm();
	void Arm();
	void FirstSkill();
	void SecondSkill();
	void PlayShieldReactMontage();
	void InitializePlayerOverlay();
	void SetHealthBar();
	void SetStaminaBar();
	void SetLevelBar();
	void PrintQuest();
	void Dodge();
	void CheckQuestProgress();
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

	UPROPERTY(EditDefaultsOnly,Category = Montages)
	UAnimMontage* FirstSkillMontage;

	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* SecondSkillMontage;
	

	float DefaultEquippedWeaponDamage;

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
	bool bForward = false;
	float StaminaResetTimerCount = 3.f;
	float StaminaTimeElapsed = 0.f;
	UPROPERTY(EditAnywhere)
	float Ragetime = 15.f;
	FVector defaultCameraLoc;
	bool bStaminaTimerOpen = false;
	bool bCanMoveCamera = false;
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

	UQuestUI* QuestOverlay;
	void ExecuteGetHit(FHitResult& BoxHit);
	void GetSkillHit(FHitResult& Skillhit);
	TArray <AActor*> IgnoreActors;
	bool SkillCanDamage;

	

	
public:
	virtual void Tick(float DeltaTime) override;
	void ResetCameraPosition();
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
