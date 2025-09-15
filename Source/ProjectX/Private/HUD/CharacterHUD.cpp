// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/CharacterHUD.h"
#include"Components\ProgressBar.h"
#include "UMG.h"
#include "Animation/WidgetAnimation.h"
#include "Animation/UMGSequencePlayer.h" // Bind için gerekli olabilir
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
		ItemNotiftyText->SetText(FText::FromString(ItemName));
	}
}

void UCharacterHUD::PlayItemReceivedTextAnimationFadeIn()
{
	if (NotifyTextAnimationFadeIn)
	{
		NotifyTextVisiblity = ESlateVisibility::Visible;
		PlayAnimation(NotifyTextAnimationFadeIn);
		ItemNotiftyText->SetVisibility(NotifyTextVisiblity);
		UE_LOG(LogTemp, Warning, TEXT(" anims work"));

	}
}

void UCharacterHUD::PlayItemReceivedTextAnimationFadeOut()
{
	if (NotifyTextAnimationFadeOut)
	{
		PlayAnimation(NotifyTextAnimationFadeOut);
	}
}

void UCharacterHUD::NotifyAnimationFinished()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("bitti")));
	NotifyTextVisiblity = ESlateVisibility::Hidden;
	ItemNotiftyText->SetVisibility(NotifyTextVisiblity);
}



void UCharacterHUD::NativeConstruct()
{
	Super::NativeConstruct();
	if (NotifyTextAnimationFadeIn)
	{
		FWidgetAnimationDynamicEvent EndDelegate;
		EndDelegate.BindUFunction(this, FName("NotifyAnimationFinished"));
		BindToAnimationFinished(NotifyTextAnimationFadeOut, EndDelegate);
	}
}


	

