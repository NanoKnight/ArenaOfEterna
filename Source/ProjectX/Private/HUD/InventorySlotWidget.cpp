// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/InventorySlotWidget.h"
#include "Components/Image.h"
#include"../WarriorCharacter.h"
#include"Components\Button.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/DragDropOperation.h"
#include"HUD\EqiupmentSlotWidget.h"
#include"Items\BaseItem.h"
#include"HUD\InventoryWidget.h"
#include"Components\InventorySystem\InventoryComponent.h"
#include "Components/TextBlock.h"

void UInventorySlotWidget::SetUp(const FInventoryStruct& NewItem)
{
	if (NewItem.ItemName.IsEmpty())
	{
		return;
	}

	Item = NewItem;
	//if (!Item.ItemIcon && Item.ItemName.IsEmpty()) return;


	if (ItemIcon)
	{
		if (Item.ItemIcon)
		{
			ItemIcon->SetBrushFromTexture(Item.ItemIcon);

		}
		
	}

	if (ItemName)
	{
		ItemName->SetText(FText::FromString(Item.ItemName));
	}
}

void UInventorySlotWidget::SetSlotIndex(int32 NewIndex)
{
	SlotIndex = NewIndex;
}

void UInventorySlotWidget::OnItemClicked()
{
	AWarriorCharacter* Warrior = Cast<AWarriorCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	
	if (Warrior)
	{	
			Warrior->EquipItem(Item);
			
	}

}



FReply UInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);


	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
			

	}

	return FReply::Unhandled();
}

void UInventorySlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{

	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	if (Item.ItemName.IsEmpty())
	{
		
		return;	

	}

	UDragDropOperation* DragDropOp = UWidgetBlueprintLibrary::CreateDragDropOperation(UDragDropOperation::StaticClass());
	if (DragDropOp)
	{
		DragDropOp->Payload = this;
		DragDropOp->DefaultDragVisual = this;
		DragDropOp->Pivot = EDragPivot::TopLeft;
		OutOperation = DragDropOp;



	}
}

bool UInventorySlotWidget::NativeOnDrop(const FGeometry& Geometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	//KODLARI DÜZElt burdaki moduler yap///
	if (!InOperation || !InOperation->Payload)
	{
		return false;
	}

	UEqiupmentSlotWidget* DraggedItem = Cast<UEqiupmentSlotWidget>(InOperation->Payload);
	if (DraggedItem)
	{

		AWarriorCharacter* Warrior = Cast<AWarriorCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		
		
		if (!DraggedItem->EquippedItem.ItemName.IsEmpty() && Item.ItemName.IsEmpty())
		{

			int32 TargetIndex = this->SlotIndex;
		
			if (TargetIndex != -1)
			{
				Item = DraggedItem->EquippedItem;
				SetUp(DraggedItem->EquippedItem);
				Warrior->GetInventoryComponent()->InventoryItems[TargetIndex] = DraggedItem->EquippedItem;


				Warrior->GetInventoryComponent()->UnEquipItem(DraggedItem->EquippedItem, DraggedItem->EquippedItemActor);

				DraggedItem->EquippedItem = FInventoryStruct();
				DraggedItem->EquippedItemActor = nullptr;
				DraggedItem->SetDefaultWeaponIcon();

				return true;
			}
		
		}
		else if (!Item.ItemName.IsEmpty()&& !DraggedItem->EquippedItem.ItemName.IsEmpty())
		 {
			 FInventoryStruct TempItem = DraggedItem->EquippedItem;
			 if (Item.ItemTypes == DraggedItem->ItemTypes)
			 {
				 Warrior->GetInventoryComponent()->UnEquipItem(DraggedItem->EquippedItem, DraggedItem->EquippedItemActor);
				 Warrior->GetInventoryComponent()->InventoryItems.Add(DraggedItem->EquippedItem);

				 // 2. Yeni itemi kuþan
				 DraggedItem->EquipmentIcon->SetBrushFromSoftTexture(Item.ItemIcon);
				 DraggedItem->EquippedItem = Item;
				 DraggedItem->EquippedItemActor = EquippedItemActor;
				 Warrior->GetInventoryComponent()->EquipItem(DraggedItem->EquippedItem);

				 // 3. Slot'u güncelle
				 ItemIcon->SetBrushFromSoftTexture(TempItem.ItemIcon);
				 ItemName->SetText(FText::FromString(TempItem.ItemName));
				 Item = TempItem;
			 }
		
		 }
	
		return true;


	}
	

	// 1. Sürüklenen slotu al
	UInventorySlotWidget* DraggedSlot = Cast<UInventorySlotWidget>(InOperation->Payload);
	if (!DraggedSlot || DraggedSlot == this) return false;

	// 2. Componenti bul
	AWarriorCharacter* Warrior = Cast<AWarriorCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (!Warrior || !Warrior->GetInventoryComponent()) return false;

	// 3. TAÞIMA ÝÞLEMÝNÝ YAP (Artýk swap yok, direkt move!)
	Warrior->GetInventoryComponent()->MoveItem(DraggedSlot->SlotIndex, this->SlotIndex);
	Warrior->GetInventoryComponent()->OldSlotIndex = SlotIndex;
	return true;
	//return false;
}

bool UInventorySlotWidget::IfInventorySlotItemIsValid()
{
	return !Item.ItemName.IsEmpty();
}
