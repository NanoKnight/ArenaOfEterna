// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include"../Characters/BaseCharacter.h"
#include"../CharacterTypes.h"
#include "Enemy.generated.h"


class UHealthBarComponent;
class AWarriorCharacter;
class UPawnSensingComponent;


UCLASS()
class PROJECTX_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void InitializeEnemy();
	virtual void Die() override;
	void SetEnemyDead();
	void AddKilledEnemy();
	void SpawnExperience();
	virtual void Attack() override;
	virtual void AttackEnd() override;
	virtual bool CanAttack() override;
	virtual void HandleDamage(float DamageAmount) override;
	


	
public:
    AEnemy();
	FString EnemyName;
	bool IsDead();
    /* <AActor>  */
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Destroyed() override;
	/*</AActor>*/

	/* <IHitInterface> */
	virtual void GetHit_Implementation(const FVector& ImpactPoint,AActor* Hitter) override;
	/* </IHitInterface> */

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
	bool IsOutsideCombatRadius();
	bool IsOutsideAttackRadius();
	bool IsInsideAttackRadius();
	bool IsChasing();
	bool IsAttacking();
	
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
	double PatrolRadius = 200.f;

	FTimerHandle PatrolTimer;

	UPROPERTY(EditDefaultsOnly, Category = "AI Navigation")
	float PatrolWaitTimeMin = 2.f;

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
	FTimerHandle AttackTimer;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackMin = 0.5f;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackMax = 1.f;
	
	UPROPERTY(EditAnywhere,Category="Combat")
	float RunSpeed = 300.f;
	
	UPROPERTY(EditAnywhere, Category = Combat)
	float DeathLifeSpan = 3.f;

	UPROPERTY(EditAnywhere, Category = Combat)
	TSubclassOf<class AExperiencePoint> Experience;

	
	
};
