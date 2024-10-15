// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include"../SaveGames/EternaSaveGame.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include"../Enemy/Enemy.h"
#include"../SaveGames/EternaSaveGame.h"
#include"../Interfaces/RespawnEnemyInterface.h"
#include "ArenaGameMode.generated.h"

class AEnemy;
class UAttributeComponent;
/**
 * 
 */
UCLASS()
class PROJECTX_API AArenaGameMode : public AGameModeBase, public IRespawnEnemyInterface
{
	GENERATED_BODY()

public:
	AArenaGameMode();
	void SaveGame();
	void LoadGame();
	void RemoveEnemyFromWorld();
	void RemoveItemFormWorld();
	void IncrementEnemyAlive();
	void DecrementEnemyAlive();
	void CheckEnemy();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SaveGame")
	TArray<FString>KilledEnemiesNames;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SaveGame")
	TArray<FString> AddedItems;
	UAttributeComponent* Attributes;

	UPROPERTY(VisibleAnywhere)
	int32 EnemyAlive;

	UPROPERTY(VisibleAnywhere)
     int32 NextWaveEnemyCount;

protected:

	virtual void BeginPlay()override;
	virtual void RespawnEnemy()override;

	class AEnemySpawner* EnemySpawner;

private:


	


};
