// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "QuestStruct.generated.h"

/**
 * 
 */
class AEnemy;
class ABoss;

UENUM(BlueprintType)
enum class EQuestType : uint8
{
	GoToLocation UMETA(DisplayName = "Go to Location"),
	KillEnemies UMETA(DisplayName = "Kill Enemies"),
	DestroyBoss UMETA(DisplayName = "Destroy Boss"),
	PickupItem UMETA(DisplayName = "PickUp Item"),
	WearItem UMETA(DisplayName = "Wear Item")



};




USTRUCT(BlueprintType)
struct PROJECTX_API FQuestStruct : public FTableRowBase
{	

GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quests")
	FString QuestName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quests")
	FString QuestDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString QuestItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AEnemy> EnemyClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABoss> BossClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quests")
	bool bIsQuestCompleted = false;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EQuestType QuestType;

	// Go to Location specific
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector TargetLocation;

	// Kill Enemies specific
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TargetKillCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentKillCount;



	FQuestStruct();
	~FQuestStruct();
};
