// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/ItemInteractionWidget.h"
#include"Components\TextBlock.h"
#include "UMG.h"

void UItemInteractionWidget::SetInteractionValues(FString ItemName, int32 ItemAbility, EItemTypes ItemTypes)
{

	FString SItemName = FString::Printf(TEXT("Item Name : %s "),*ItemName);
	FText TextItemName = FText::FromString(SItemName);

	ItemNameText->SetText(TextItemName);

	if (ItemTypes == EItemTypes::Weapon)
	{
		FString SItemAbility = FString::Printf(TEXT("Damage : %d"), ItemAbility);
		FText TextItemability = FText::FromString(SItemAbility);
		ItemAbilityText->SetText(TextItemability);

	}
	else
	{
		FString SItemAbility = FString::Printf(TEXT("Defense : %d"), ItemAbility);
		FText TextItemability = FText::FromString(SItemAbility);
		ItemAbilityText->SetText(TextItemability);

	}
}
