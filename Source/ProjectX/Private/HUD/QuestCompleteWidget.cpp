// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/QuestCompleteWidget.h"
#include"Components\TextBlock.h"

void UQuestCompleteWidget::SetQuestText(const FString& QuestName)
{
	NewQuestText->SetText(FText::FromString(QuestName));
}

void UQuestCompleteWidget::PlayFadeInAnimation()
{

	PlayAnimation(FadeIn);
}

void UQuestCompleteWidget::PlayFadeOutAnimation()
{
	PlayAnimation(FadeOut);
}
