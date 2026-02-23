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
class ASpawnManager;
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
	void PlayCombatSound();
	void StopCombatSound();
	void RemoveEnemyFromWorld();
	void RemoveItemFormWorld();
	void IncrementEnemyAlive();
	void DecrementEnemyAlive();
	void CheckEnemy();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SaveGame")
	TArray<FString>KilledEnemiesNames;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SaveGame")
	TArray<FString> AddedItems;

	UPROPERTY()
	UAttributeComponent* Attributes;

	UPROPERTY(VisibleAnywhere)
	int32 EnemyAlive;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 ChasedEnemies;

	UPROPERTY(VisibleAnywhere)
     int32 NextWaveEnemyCount;
	 
	 UPROPERTY(VisibleAnywhere)
	 int32 WaveCount = 1;
	 UPROPERTY()
	 FTimerHandle WaveStartTimer;

	 UPROPERTY(BlueprintReadOnly)
	 bool WaveStarted;
protected:

	virtual void BeginPlay()override;
	virtual void RespawnEnemyStart_Implementation() override;

	UPROPERTY(EditAnywhere)
	USoundBase* CombatSound;

	UPROPERTY()
	UAudioComponent* CombatAudioComponent;

	bool IsCombatSoundPlaying;

	class AEnemySpawner* EnemySpawner;

private:

	ASpawnManager* SpawnManager;

	void RespawnEnemy();


};
