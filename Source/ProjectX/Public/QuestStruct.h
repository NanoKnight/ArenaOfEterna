// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "QuestStruct.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct PROJECTX_API FQuestStruct : public FTableRowBase
{	

GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quests")
	FString QuestName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quests")
	FString QuestDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quests")
	bool bIsQuestCompleted = false;


	FQuestStruct();
	~FQuestStruct();
};
