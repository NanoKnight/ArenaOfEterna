// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/DropItemArea.h"
#include "Blueprint/DragDropOperation.h"
#include"HUD/InventorySlotWidget.h"
#include"HUD/EqiupmentSlotWidget.h"
#include"Components\Image.h"
#include "Components\InventorySystem\InventoryComponent.h"
#include"../WarriorCharacter.h"


bool UDropItemArea::NativeOnDrop(const FGeometry& Geometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	if (!InOperation || !InOperation->Payload) return false;

	UEqiupmentSlotWidget* DraggedEquipmentSlot = Cast<UEqiupmentSlotWidget>(InOperation->Payload);
	UInventorySlotWidget* DraggedInventorySlot = Cast<UInventorySlotWidget>(InOperation->Payload);
	AWarriorCharacter* WarriorRef = Cast<AWarriorCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (DraggedEquipmentSlot)
	{
		if (WarriorRef)
		{
			DraggedEquipmentSlot->EquipmentIcon->SetBrushFromSoftTexture(DraggedEquipmentSlot->EquipmentImage);
			WarriorRef->GetInventoryComponent()->DropItem(DraggedEquipmentSlot->EquippedItem);
			WarriorRef->GetInventoryComponent()->UnEquipItem(DraggedEquipmentSlot->EquippedItem , DraggedEquipmentSlot->EquippedItemActor);
			DraggedEquipmentSlot->EquippedItem = FInventoryStruct();
			DraggedEquipmentSlot->EquippedItemActor = nullptr;
		}
		return true;

		
	}
	 if(DraggedInventorySlot)
	{
		if (WarriorRef)
		{
			DraggedInventorySlot->ItemIcon->SetBrushFromSoftTexture(DraggedInventorySlot->ImageIconAsset);
			WarriorRef->GetInventoryComponent()->DropItem(DraggedInventorySlot->Item);
			DraggedInventorySlot->Item = FInventoryStruct();
			//DraggedEquipmentSlot->EquippedItemActor = nullptr;
		}
		return true;
	}


	return false;
}
