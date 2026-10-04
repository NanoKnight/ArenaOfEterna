// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/CurrentBossOverlayWidget.h"
#include"Components\TextBlock.h"
#include"Components\ProgressBar.h"

void UCurrentBossOverlayWidget::SetCurrentBossName(FString BossName)
{
	if (CurrentBossName)
	{
		const FText TextRef = FText::FromString(BossName);
		CurrentBossName->SetText(TextRef);
	}
}

void UCurrentBossOverlayWidget::SetCurrentBossHealthBar(float Percent)
{
	if (CurrentBossHealthBar)
	{
		CurrentBossHealthBar->SetPercent(Percent);
	}
}
