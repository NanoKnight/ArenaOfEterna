// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/InventoryWidget.h"
#include "Components/GridPanel.h"
#include"Components\GridSlot.h"
#include"HUD\EqiupmentSlotWidget.h"
#include "Blueprint/DragDropOperation.h"
#include"Components\InventorySystem\InventoryComponent.h"
#include"../WarriorCharacter.h"
#include "HUD/InventorySlotWidget.h"

bool UInventoryWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
/********************************BU FONKSIYONU OPTIMIZE ETMEYE CALIS************************************/

	CheckHoveringOnInventoryList(InDragDropEvent);
	if (IsHoveringOnInventoryList())
	{
		UEqiupmentSlotWidget* DraggedItem = Cast<UEqiupmentSlotWidget>(InOperation->Payload);
		if (DraggedItem)
		{
			AWarriorCharacter* Warrior = Cast<AWarriorCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
			if (Warrior && !DraggedItem->EquippedItem.ItemName.IsEmpty())
			{
				Warrior->GetInventoryComponent()->UnEquipItem(DraggedItem->EquippedItem, DraggedItem->EquippedItemActor);

				TArray<FInventoryStruct> InventoryArray;

				UInventorySlotWidget* ItemSlot = CreateWidget<UInventorySlotWidget>(this, InventorySlotWidgetClass);

					if (ItemSlot)
					{

						ItemSlot->SetUp(DraggedItem->EquippedItem);
						Warrior->GetInventoryComponent()->InventoryItems.Add(DraggedItem->EquippedItem);
						//InventoryList->AddChild(ItemSlot);

					
						UGridSlot* GridSlot = InventoryList->AddChildToGrid(ItemSlot);
						if (GridSlot)
						{

					 	   UpdateInventoryDisplay(Warrior->GetInventoryComponent()->InventoryItems);
					 	   GridSlot->SetPadding(FMargin(5.f));

						}

					}
				DraggedItem->SetDefaultWeaponIcon();
				DraggedItem->EquippedItem = FInventoryStruct();

			}
		}
	}
	return true;
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




void UInventoryWidget::UpdateInventoryDisplay(const TArray<FInventoryStruct>& InventoryItems)
{

	if (!InventoryList || !InventorySlotWidgetClass)return;
	InventoryList->ClearChildren();

	for (int32 i = 0; i < InventoryItems.Num(); i++)
	{
		const FInventoryStruct& Item = InventoryItems[i];
		UInventorySlotWidget* ItemSlot = CreateWidget<UInventorySlotWidget>(this, InventorySlotWidgetClass);

		if (ItemSlot)
		{
			ItemSlot->SetUp(Item);
			InventoryList->AddChild(ItemSlot);
			
			int32 Row = i / 6;
			int32 Column = i % 6;
			UGridSlot* GridSlot = InventoryList->AddChildToGrid(ItemSlot, Row, Column);
			if (GridSlot)
			{
		
				
					GridSlot->SetPadding(FMargin(5.f));
				
			}

			//InventoryListt->AddChild(ItemSlot);
		
		
		}
	}
}

FReply UInventoryWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FKey PressedKey = InKeyEvent.GetKey();
	if (PressedKey == EKeys::I)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString(TEXT("CalledClose")));
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




