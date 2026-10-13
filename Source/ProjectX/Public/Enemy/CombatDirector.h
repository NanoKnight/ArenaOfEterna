#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CombatDirector.generated.h"

class AEnemy;
class AWarriorCharacter;

UCLASS()
class PROJECTX_API ACombatDirector : public AActor
{
	GENERATED_BODY()

public:
	ACombatDirector();

	virtual void Tick(float DeltaTime) override;

	
	void RegisterEnemy(AEnemy* Enemy);

	void ReleaseAttacker(AEnemy* Enemy);


	void SelectAttackers();
	
	void ReleaseAttackSlot(AEnemy* Enemy);
	
	bool HasAttackPermission(const AEnemy* Enemy)const;



	FVector GetSurroundLocation(AEnemy* Enemy) const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TArray<AEnemy*> RegisteredEnemies;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	AEnemy* OldAttacker = nullptr;

	UPROPERTY()
	TArray <AEnemy*> ActiveAttackers;
	int32 nextAttackerIndex;

	UPROPERTY()
	float DoubleAttackChance = 0.4f;

	UPROPERTY()
	int32 MaxAttackers = 2;
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")


	AWarriorCharacter* WarriorRef;

protected:
	virtual void BeginPlay() override;

private:
	void UpdateCombat();

	UPROPERTY(EditAnywhere, Category = "Combat|Surround")
	float SurroundRadius = 150.f;

	UPROPERTY(EditAnywhere, Category = "Combat|Surround")
	float SurroundRotationSpeed = 15.f;

	UPROPERTY(EditAnywhere, Category = "Combat|Surround")
	float CombatUpdateInterval = 0.1f;

	float SurroundAngleOffset = 0.f;

	FTimerHandle UpdateCombatTimer;
	 
	
};