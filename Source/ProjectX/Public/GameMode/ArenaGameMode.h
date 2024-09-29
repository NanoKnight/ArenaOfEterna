// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include"../SaveGames/EternaSaveGame.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include"../Enemy/Enemy.h"
#include"../SaveGames/EternaSaveGame.h"
#include "ArenaGameMode.generated.h"

class AEnemy;
class UAttributeComponent;
/**
 * 
 */
UCLASS()
class PROJECTX_API AArenaGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AArenaGameMode();
	void SaveGame();
	void LoadGame();
	void RemoveEnemyFromWorld();
	void RemoveItemFormWorld();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SaveGame")
	TArray<FString>KilledEnemiesNames;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SaveGame")
	TArray<FString> AddedItems;
	UAttributeComponent* Attributes;

protected:

	virtual void BeginPlay()override;

private:
	


};
