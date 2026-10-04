// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CurrentBossOverlayWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTX_API UCurrentBossOverlayWidget : public UUserWidget
{
	GENERATED_BODY()

	public:


		UFUNCTION()
		void SetCurrentBossName(FString BossName);

		UFUNCTION()
		void SetCurrentBossHealthBar(float Percent);



	protected:




	private:

		UPROPERTY(meta = (BindWidget))
		class UProgressBar* CurrentBossHealthBar;

		UPROPERTY(meta = (BindWidget))
		class UTextBlock* CurrentBossName;



	
};
