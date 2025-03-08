// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/InventoryWidget.h"
#include "Components/GridPanel.h"
#include"Components\GridSlot.h"
#include"HUD\EqiupmentSlotWidget.h"
#include "Blueprint/DragDropOperation.h"
#include"./Components/TextBlock.h"
#include"Components\InventorySystem\InventoryComponent.h"
#include"../WarriorCharacter.h"
#include "HUD/InventorySlotWidget.h"



void UInventoryWidget::NativeConstruct()
{
}



void UInventoryWidget::CheckHoveringOnInventoryList(const FDragDropEvent& InDragDropEvent)
{
	if (InventoryList)
	{
		FGeometry InventoryListGeometry = InventoryList->GetCachedGeometry();

		FVector2D MousePosition = InDragDropEvent.GetScreenSpacePosition();

		if (InventoryListGeometry.IsUnderLocation(MousePosition))
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString(TEXT("Hovering")));

			bIsHovering = true;
		}
		else
		{
			bIsHovering = false;
		}

	}
}

bool UInventoryWidget::IsHoveringOnInventoryList()
{
	return bIsHovering == true;
}




void UInventoryWidget::SwapItems(int32 FromIndex, int32 ToIndex)
{

	if (SlotIndices.IsValidIndex(FromIndex)&& SlotIndices.IsValidIndex(ToIndex))
	{
		SlotIndices.Swap(FromIndex, ToIndex);
		
		AWarriorCharacter* Warrior = Cast<AWarriorCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if (Warrior && Warrior->GetInventoryComponent())
		{
			Warrior->GetInventoryComponent()->InventoryItems.Swap(FromIndex, ToIndex);
		}
		UpdateInventoryDisplay(Warrior->GetInventoryComponent()->InventoryItems);
	}
}

void UInventoryWidget::UpdateInventoryDisplay(const TArray<FInventoryStruct>& InventoryItems)
{

	if (!InventoryList || !InventorySlotWidgetClass) return;

	InventoryList->ClearChildren();
	InventorySlots.Empty();
	
	const int32 MaxSlots = 20;    

	if (SlotIndices.Num() != MaxSlots)
	{
		SlotIndices.Empty();
		for (int32 i = 0; i < MaxSlots; i++)
		{
			SlotIndices.Add(i);
		}
	}

	for (int32 i = 0; i < MaxSlots; i++)
	{
		UInventorySlotWidget* ItemSlot = CreateWidget<UInventorySlotWidget>(this, InventorySlotWidgetClass);
		if (ItemSlot)
		{

			int32 RealIndex = SlotIndices.IsValidIndex(i) ? SlotIndices[i] : i;

			if (InventoryItems.IsValidIndex(RealIndex) && !InventoryItems[RealIndex].ItemName.IsEmpty())
			{
				ItemSlot->SetUp(InventoryItems[i]);
			}
			else
			{
				//ItemSlot->SetUp(FInventoryStruct());
			}

			InventoryList->AddChild(ItemSlot);

			int32 Row = i / 7;
			int32 Column = i % 7;
			UGridSlot* GridSlot = InventoryList->AddChildToGrid(ItemSlot, Row, Column);
			if (GridSlot)
			{
				GridSlot->SetPadding(FMargin(10.f));
			}
			InventorySlots.Add(ItemSlot);
		}
	}
	
}

FReply UInventoryWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FKey PressedKey = InKeyEvent.GetKey();
	if (PressedKey == EKeys::I)
	{
		AWarriorCharacter* Warrior = Cast<AWarriorCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if (Warrior)
		{
			APlayerController* Controller = GetWorld()->GetFirstPlayerController();
			Warrior->GetInventoryComponent()->ToggleInventory(Controller);

		}
		return FReply::Handled();
	}

	return FReply::Unhandled();

}
