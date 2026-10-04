// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestCompleteWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTX_API UQuestCompleteWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Quests")
	void SetQuestText(const FString& QuestName);

	UFUNCTION()
	void PlayFadeInAnimation();

	UFUNCTION()
	void PlayFadeOutAnimation();

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* FadeIn;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* FadeOut;

protected:
	

private:
	UPROPERTY(Meta = (BindWiget))
	class UTextBlock* NewQuestText;
};
