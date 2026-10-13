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

	// Combat sistemine enemy ekler.
	void RegisterEnemy(AEnemy* Enemy);

	// Enemy öldüðünde veya oyuncudan uzaklaþtýðýnda tamamen kaldýrýr.
	void ReleaseAttacker(AEnemy* Enemy);

	// Saldýrý bitince yalnýzca saldýrý sýrasýný deðiþtirir.
	void ReleaseAttackPermission(AEnemy* Enemy);

	void SelectAttacker();
	
	void SelectNextAttacker();

	FVector GetSurroundLocation(AEnemy* Enemy) const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	TArray<AEnemy*> RegisteredEnemies;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	AEnemy* OldAttacker = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	AEnemy* CurrentAttacker = nullptr;

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
	float CombatUpdateInterval = 1.f;

	float SurroundAngleOffset = 0.f;

	FTimerHandle UpdateCombatTimer;
	 
	int32 AttackerDirection = 1;
};