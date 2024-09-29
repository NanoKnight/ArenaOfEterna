// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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

};
