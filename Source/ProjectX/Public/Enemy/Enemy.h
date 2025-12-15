
#pragma once

#include "CoreMinimal.h"
#include"../Characters/BaseCharacter.h"
#include"../EnumStates.h"
#include"../Interfaces/SkillHitInterface.h"
#include "Enemy.generated.h"


class UHealthBarComponent;
class AWarriorCharacter;
class UPawnSensingComponent;
class AEnemySpawner;

UCLASS()
class PROJECTX_API AEnemy : public ABaseCharacter, public ISkillHitInterface
{ 
	GENERATED_BODY()

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void InitializeEnemy();
	virtual void Die() override;
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
	virtual void HandleDamage(float DamageAmount) override;
	virtual void PlayHitSound(const FVector& ImpactPoint) ;



	
public:
    AEnemy();
	FString EnemyName;
	bool IsDead();
    /* <AActor>  */
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Destroyed() override;
	void SetRagdoll();
	/*</AActor>*/

	/* <IHitInterface> */
	virtual void GetHit_Implementation(const FVector& ImpactPoint,AActor* Hitter) override;
	/* </IHitInterface> */

	virtual void SkillHit(const FVector& ImpactPoint, AActor* Hitter) override;

	UFUNCTION(BlueprintCallable)
	void ResetEnemyState();



	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState;





private:

	/* AI Behavior */
	void SpawnDefaultWeapon();
	void CheckPatrolTarget();
	void CheckCombatTarget();
	void PatrolTimerFinished();
	void HideHealthBar();
	void ShowHealthBar();
	void LoseInterest();
	void StartPatrolling();
	void ChaseTarget();
	void ResetRagdoll();
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
	bool InTargetRange(AActor* Target, double Radius);
	void MoveToTarget(AActor* Target);
	AActor* ChoosePatrolTarget();

	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);

	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* HealthBarWidget;

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensing;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> WeaponClass;

	UPROPERTY(EditAnywhere)
	double CombatRadius = 1500.f;

	UPROPERTY(EditAnywhere)
	double AttackRadius = 200.f;

	UPROPERTY(EditAnywhere)
	bool InfiniteEnemy;

	UPROPERTY(EditAnywhere)
	double PatrolRadius = 200.f;

	FTimerHandle PatrolTimer;

	FTimerHandle SpawnExperienceTimer;

	UPROPERTY(EditDefaultsOnly, Category = "AI Navigation")
	float PatrolWaitTimeMin = 2.f;

	UPROPERTY()
	int32 ItemSpawnRate = 5;
	
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

	FTimerHandle AttackTimer;
	FTimerHandle RagdollTimer;
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


	UPROPERTY(EditAnywhere, Category = "Combat");
	int32 ExperiencePoint;

	UPROPERTY(EditAnywhere, Category = Combat)
	TSubclassOf<class AExperiencePoint> Experience;

	UPROPERTY(EditDefaultsOnly, category = Montages)
	UAnimMontage* SkillDamageMontage;

	AWarriorCharacter* WarriorCharacter;
	
	
};
