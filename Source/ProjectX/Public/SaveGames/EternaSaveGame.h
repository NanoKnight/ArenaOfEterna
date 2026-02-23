// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include"../Structs/InventoryStruct.h"
#include "EternaSaveGame.generated.h"

class AWeapon;

UCLASS()
class PROJECTX_API UEternaSaveGame : public USaveGame
{
	GENERATED_BODY()
	public:
		
    UPROPERTY(EditDefaultsOnly)
	FVector PlayerLocation;

	UPROPERTY(EditDefaultsOnly)
	int32 CurrentQuestIndex;
	
	UPROPERTY(EditdefaultsOnly)
	float Health;

	UPROPERTY(EditdefaultsOnly)
	float Exp;

	UPROPERTY(EditdefaultsOnly)
	float MaxExp;

	UPROPERTY(EditdefaultsOnly)
	float Stamina;

	UPROPERTY(EditDefaultsOnly)
	int32 Level;

	UPROPERTY(EditDefaultsOnly)
	int32 Gold;

	UPROPERTY(EditDefaultsOnly)
	int32 EnemyNextWaveCount;


	UPROPERTY(EditDefaultsOnly)
	int32 WaveCount = 1;

	UPROPERTY(EditDefaultsOnly)
	TArray<FInventoryStruct> SavedInventoryItems;

	UPROPERTY(EditDefaultsOnly)
	TArray<FInventoryStruct> SavedEquipItems;



	/**/UPROPERTY(EditAnywhere, Category = Weapon)
	TSubclassOf<AWeapon> EquippedWeapon;

	UPROPERTY(EditAnywhere, Category = Weapon)
	AWeapon* weapon;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SaveGame")
	TArray<FString> KilledEnemiesName;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SaveGame")
	TArray<FString> AddedItems;

	void AddKilledEnemy(FString EnemyName);
	void AddItems(FString AddedItemsName);

	
};
