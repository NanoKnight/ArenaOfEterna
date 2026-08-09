
#pragma once

#include "CoreMinimal.h"
#include"../Characters/BaseCharacter.h"
#include"../EnumStates.h"
#include"../Interfaces/SkillHitInterface.h"
#include "Enemy.generated.h"


class UHealthBarComponent;
class UWidgetComponent;
class AWarriorCharacter;
class UPawnSensingComponent;
class AEnemySpawner;
class ACombatDirector;

UCLASS()
class PROJECTX_API AEnemy : public ABaseCharacter, public ISkillHitInterface
{ 
	GENERATED_BODY()

protected:


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void InitializeEnemy();
	virtual void Die() override;
	virtual void InitializeEquipItems() override;
	void SpawnEquipedItemsToWorld();
	void DestroyEquipItems();
	void IncreaseQuestKillCount();
	void SetEnemyDead();
	void AddKilledEnemy();
	void SpawnExperience();
	UFUNCTION()
	void RespawnInfiniteEnemy(AEnemySpawner* SpawnerActor);
	virtual void Attack() override;
	virtual void AttackEnd() override;
	virtual bool CanAttack() override;
	virtual void ChaseTarget();
	void deneme();
	virtual void HandleDamage(float DamageAmount) override;
	virtual void PlayHitSound(const FVector& ImpactPoint) ;
	void MoveToRandomLocation();

	FTimerHandle RandomMoveTimer;

	FTimerHandle UpdateCombatTimer;


	bool Chased;

	FTimerHandle OutFreezeTimer;

	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);

	UMaterialInterface* OldMat;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* FreezeMat;

	
public:
    AEnemy();
	FString EnemyName;
	bool IsDead();
	void MoveToSurroundLocation(const FVector& Location);
	bool HasAttackPermission() const;

    /* <AActor>  */
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void MoveToTarget(AActor* Target);
	virtual void Destroyed() override;
	void SetRagdoll();
	void SetStun();
	void ParryReset();
	void SetEnemyFreeze();
	void OutFreeze();
	/*</AActor>*/

	/* <IHitInterface> */
	virtual void GetHit_Implementation(const FVector& ImpactPoint,AActor* Hitter) override;
	void EndRecoveryHit();
	/* </IHitInterface> */

	virtual void SkillHit(const FVector& ImpactPoint, AActor* Hitter) override;

	UFUNCTION(BlueprintCallable)
	void ResetEnemyState();

	void GetParried();

	void EndParried();


	UFUNCTION(BlueprintCallable)
	void PlayerCanParry();


	UFUNCTION(BlueprintCallable)
	void PlayerCantParry();

	

	UPROPERTY(EditAnywhere)
	EEnemyState EnemyState;

	UPROPERTY(EditAnywhere,BlueprintReadonly)
	EEnemyType EnemyType = EEnemyType::EET_Enemy;

	UPROPERTY(Editanywhere)
	bool IgnoreEnemyCount;

	UPROPERTY(EditAnywhere)
	bool CanParry;



	FORCEINLINE AActor* GetCombatTarget() const { return CombatTarget; }
	FORCEINLINE class AAIController* GetEnemyController() const { return EnemyController;}


private:

	/* AI Behavior */
	void SpawnDefaultWeapon();
	void CheckPatrolTarget();
	void CheckCombatTarget();
	void CheckCombatMusic();
	void PatrolTimerFinished();
	void HideHealthBar();
	void ShowHealthBar();
	void LoseInterest();
	void StartPatrolling();
	void ResetRagdoll();
	void BackPatrol();
	bool IsOutsideCombatRadius();
	bool IsOutsideAttackRadius();
	bool IsInsideAttackRadius();
	bool IsChasing();
	bool IsAttacking();
	bool IsStun();
	bool IsEngaged();
	void ClearPatrolTimer();
	void StartAttackTimer();
	void ClearAttackTimer();
	void UpdateCombatMovement();
	void ResetTakedHit();
	bool InTargetRange(AActor* Target, double Radius);
	AActor* ChoosePatrolTarget();


	UFUNCTION()
	void PawnHeard(APawn* SeenPawn,const FVector& Location, float Volume);

	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* ParryWidget;

	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarWidget;

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensing;

	APawn* SeenPawnRef;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> WeaponClass;

	ACombatDirector* CombatDirector;

	UPROPERTY(EditAnywhere)
	double CombatRadius = 1500.f;

	UPROPERTY(EditAnywhere)
	double AttackRadius = 200.f;

	UPROPERTY(EditAnywhere)
	bool InfiniteEnemy;

	UPROPERTY(EditAnywhere)
	double PatrolRadius = 200.f;

	FTimerHandle PatrolTimer;
	FTimerHandle ParryTimer;
	FTimerHandle ParryResetTimer;

	FTimerHandle SpawnExperienceTimer;

	UPROPERTY(EditDefaultsOnly, Category = "AI Navigation")
	float PatrolWaitTimeMin = 2.f;

	UPROPERTY()
	int32 ItemSpawnRate = 5;

	UPROPERTY()
	int32 TakedHit;
	
	UPROPERTY(EditAnywhere)
	bool SpecialCharacter;


	UPROPERTY(EditDefaultsOnly, Category = "AI Navigation")
	float PatrolWaitTimeMax = 5.f;
	UPROPERTY()
	class AAIController* EnemyController;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	AActor* PatrolTarget;

	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray <AActor*> PatrolTargets;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	
	float WalkSpeed = 125.f;

	bool Ragdoll;
	bool FrontAnim;
	bool bRecoveringFromHit = false;

	FTimerHandle AttackTimer;
	FTimerHandle RecoveryHitTimer;
	FTimerHandle RagdollTimer;
	FTimerHandle ParryStartTimer;
	FTimerHandle HideHealthBarTimer;
	FTimerHandle RespawnInfiniteEnemyTimer;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackMin = 0.5f;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackMax = 1.f;
	
	UPROPERTY(EditAnywhere,Category="Combat")
	float RunSpeed = 300.f;
	
	UPROPERTY(EditAnywhere, Category = Combat)
	float DeathLifeSpan = 3.f;
	UPROPERTY(EditAnywhere)
	float IdleTime = 0;

	UPROPERTY(EditAnywhere, Category = "Combat");
	int32 ExperiencePoint;

	UPROPERTY(EditAnywhere, Category = Combat)
	TSubclassOf<class AExperiencePoint> Experience;

	UPROPERTY(EditDefaultsOnly, category = Montages)
	UAnimMontage* SkillDamageMontage;

	AWarriorCharacter* WarriorCharacter;
	
	
};
