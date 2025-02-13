// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/EqiupmentSlotWidget.h"
#include"HUD/InventorySlotWidget.h"
#include"../WarriorCharacter.h"
#include "Blueprint/DragDropOperation.h"

#include"Components\Image.h"
bool UEqiupmentSlotWidget::NativeOnDrop(const FGeometry& InGemotry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	UInventorySlotWidget* DraggedITem = Cast<UInventorySlotWidget>(InOperation->Payload);
	if (DraggedITem)
	{
		
		AWarriorCharacter* Warrior = Cast<AWarriorCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if (Warrior)
		{
			Warrior->EquipItem(DraggedITem->Item);
		}
		return true;

	}
	return false;
}

void UEqiupmentSlotWidget::SetItemIcon(UTexture2D * NewIcon)
{
	if (EquipmentIcon && NewIcon)
	{
		EquipmentIcon->SetBrushFromTexture(NewIcon);

	}
}
