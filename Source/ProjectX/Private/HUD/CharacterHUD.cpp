// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/CharacterHUD.h"
#include"Components\ProgressBar.h"
#include"Components\TextBlock.h"

void UCharacterHUD::SetHealthBarPercent(float Percent)
{
	if (HealthBar)
	{
		HealthBar->SetPercent(Percent);
	}
}

void UCharacterHUD::SetStaminaBarPercent(float Percent)
{

	if (StaminaBar)
	{
       StaminaBar->SetPercent(Percent);
	}

}

void UCharacterHUD::SetLevelBarPercent(float Percent)
{
	if (LevelBar)
	{
		LevelBar->SetPercent(Percent);
	}
}



void UCharacterHUD::SetGoldText(int32 Gold)
{
	if (GoldText)
	{
		const FString String = FString::Printf(TEXT("%d"), Gold);
	    const FText Text = FText::FromString(String);
		GoldText->SetText(Text);
	}
}

void UCharacterHUD::SetXpText(int32 Xp)
{
	if (XPText)
	{
		const FString String = FString::Printf(TEXT("%d"), Xp);
		const FText Text = FText::FromString(String);		
		XPText->SetText(Text);
	}
}

void UCharacterHUD::SetMaxXpText(int32 MaxXPTxt)
 { 

	if (MaxText)
	{
		const FString String = FString::Printf(TEXT("%d"), MaxXPTxt);
		const FText Text = FText::FromString(String);
		MaxText->SetText(Text);
	}
	

}

void UCharacterHUD::SetLevelText(int32 Level)
{
	if (LevelText)
	{
		const FString String = FString::Printf(TEXT("%d"), Level);
		const FText Text = FText::FromString(String);
		LevelText->SetText(Text);
	}

}

void UCharacterHUD::SetReceivedItemText(FString ItemName)
{
	if (!ItemName.IsEmpty())
	{
		ReceivedItemText->SetText(FText::FromString(ItemName));
	}
}

void UCharacterHUD::PlayItemReceivedTextAnimationFadeIn()
{
	if (ItemRecivedTextAnimationFadeIn)
	{
		PlayAnimation(ItemRecivedTextAnimationFadeIn);
	}
}

void UCharacterHUD::PlayItemReceivedTextAnimationFadeOut()
{
	if (ItemRecivedTextAnimationFadeOut)
	{
		PlayAnimation(ItemRecivedTextAnimationFadeOut);

	}
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString(TEXT("PlayedAnimation")));
}
