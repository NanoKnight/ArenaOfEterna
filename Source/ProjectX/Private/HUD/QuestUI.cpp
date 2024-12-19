// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/QuestUI.h"
#include"Components\TextBlock.h"



void UQuestUI::SetQuestText(const FString& QuestName, const FString& Description)
{

    FName QuestNameAsFName(*QuestName);

    UTextBlock* QuestNameText = Cast<UTextBlock>(GetWidgetFromName(TEXT("QuestNameText")));
    UTextBlock* QuestDescriptionText = Cast<UTextBlock>(GetWidgetFromName(TEXT("QuestDescriptionText")));
        QuestText->SetText(FText::FromString(QuestName)); 
    
}


