// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/InventoryWidget.h"
#include "Components/VerticalBox.h"
#include "HUD/InventorySlotWidget.h"

void UInventoryWidget::UpdateInventoryDisplay(const TArray<FInventoryStruct>& InventoryItems)
{
	if (!InventoryList || !InventorySlotWidgetClass)return;
	InventoryList->ClearChildren();

	for (const FInventoryStruct& Item : InventoryItems)
	{	
		UInventorySlotWidget* ItemSlot = CreateWidget<UInventorySlotWidget>(this, InventorySlotWidgetClass);
		
		if (ItemSlot)
		{
			GEngine->AddOnScreenDebugMessage(0, 2.f, FColor::Cyan, FString::Printf(TEXT("slotadded")));
			ItemSlot->SetUp(Item);
			InventoryList->AddChild(ItemSlot);
		}
	}
}


