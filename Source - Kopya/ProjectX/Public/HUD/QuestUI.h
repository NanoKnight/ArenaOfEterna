// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestUI.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTX_API UQuestUI : public UUserWidget
{
	GENERATED_BODY()
public:


	UFUNCTION(BlueprintCallable, Category = "Quest UI")
	void SetQuestText(const FString& QuestName, const FString& Description);

private :
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* QuestText;

protected:

	
	
};
