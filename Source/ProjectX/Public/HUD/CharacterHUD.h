// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestUI.h"
#include "QuestCompleteWidget.h"
#include "CharacterHUD.generated.h"


/**
 * 
 */
UCLASS()
class PROJECTX_API UCharacterHUD : public UUserWidget
{
	GENERATED_BODY()


public:

	void SetHealthBarPercent(float Percent);
	void SetStaminaBarPercent(float Percent);
	void SetLevelBarPercent(float Percent);
	void SetGoldText(int32 Gold);
	void SetXpText(int32 Xp);
	void SetMaxXpText(int32 MaxXPTxt);
	void SetLevelText(int32 Level);
	void SetReceivedItemText(FString ItemName);
	void PlayItemReceivedTextAnimationFadeIn();
	void PlayItemReceivedTextAnimationFadeOut();
	void SetReceivedItemTextVisibility(ESlateVisibility ItemVisibility);

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* QuestCompleteFadeIn;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* QuestCompleteFadeOut;

	UPROPERTY(meta = (BindWidgetAnim),Transient)
	UWidgetAnimation* ItemRecivedTextAnimationFadeIn;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* ItemRecivedTextAnimationFadeOut;

	FORCEINLINE UQuestUI* GetQuestOverlay() { return QuestUI; }
	FORCEINLINE UQuestCompleteWidget* GetQuestCompleteWidget() { return QuestCompleteWidget; }

private:

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* StaminaBar;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* LevelBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* GoldText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* XPText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MaxText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* LevelText;

	UPROPERTY(meta =(BindWidget))
	class UTextBlock* ReceivedItemText;

	UPROPERTY(meta = (BindWidget))
	class UQuestUI* QuestUI;

	UPROPERTY(meta = (BindWidget))
	class UQuestCompleteWidget* QuestCompleteWidget;




};
