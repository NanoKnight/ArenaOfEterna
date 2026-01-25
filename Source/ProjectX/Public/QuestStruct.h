// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "QuestStruct.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EQuestType : uint8
{
	GoToLocation UMETA(DisplayName = "Go to Location"),
	KillEnemies UMETA(DisplayName = "Kill Enemies"),
	PickupItem UMETA(DisplayName = "PickUpItem")


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
