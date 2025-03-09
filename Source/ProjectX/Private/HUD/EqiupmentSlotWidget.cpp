// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/EqiupmentSlotWidget.h"
#include"HUD/InventorySlotWidget.h"
#include"../WarriorCharacter.h"
#include"./Components/InventorySystem/InventoryComponent.h"
#include "Blueprint/DragDropOperation.h"
#include"Components\TextBlock.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include"Components\Image.h"


bool UEqiupmentSlotWidget::NativeOnDrop(const FGeometry& InGemotry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	UInventorySlotWidget* DraggedITem = Cast<UInventorySlotWidget>(InOperation->Payload);
	if (DraggedITem && DraggedITem->Item.ItemTypes == ItemTypes)
	{
		AWarriorCharacter* Warrior = Cast<AWarriorCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if (Warrior)
		{
			if (!EquippedItem.ItemName.IsEmpty())
			{
				FInventoryStruct TempItem = DraggedITem->Item;

				Warrior->GetInventoryComponent()->UnEquipItem(EquippedItem,DraggedITem->EquippedItemActor);
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("dolu")));
				EquipmentIcon->SetBrushFromSoftTexture(TempItem.ItemIcon);
				Warrior->GetInventoryComponent()->EquipItem(TempItem);
				DraggedITem->ItemIcon->SetBrushFromSoftTexture(EquippedItem.ItemIcon);
				DraggedITem->ItemName->SetText(FText::FromString(EquippedItem.ItemName));
				DraggedITem->Item = EquippedItem;
				EquippedItem = TempItem;


			}

			else
			{
				Warrior->EquipItem(DraggedITem->Item);
				Warrior->GetInventoryComponent()->RemoveFormInventory(DraggedITem->Item);
				EquippedItem = DraggedITem->Item;
				EquippedItemActor = DraggedITem->EquippedItemActor;
				EquipmentIcon->SetBrushFromSoftTexture(DraggedITem->Item.ItemIcon);
				DraggedITem->ItemIcon->SetBrushFromSoftTexture(DraggedITem->ImageIconAsset);
				DraggedITem->Item = FInventoryStruct();
				DraggedITem->ItemName->SetText(FText::GetEmpty());
			}
			
			
				
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

void UEqiupmentSlotWidget::NativeConstruct()
{
	
	SetDefaultWeaponIcon();
}

void UEqiupmentSlotWidget::SetDefaultWeaponIcon()
{
	
		EquipmentIcon->SetBrushFromTexture(EquipmentImage);
	
}

FReply UEqiupmentSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	 FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	 if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	 {
		 return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	 }

	 return FReply::Unhandled();
}

void UEqiupmentSlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{

	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	UDragDropOperation* DragDropOp = UWidgetBlueprintLibrary::CreateDragDropOperation(UDragDropOperation::StaticClass());
	if (DragDropOp)
	{
		DragDropOp->Payload = this;
		DragDropOp->DefaultDragVisual = this;
		DragDropOp->Pivot = EDragPivot::MouseDown;		
		OutOperation = DragDropOp;
	}
}
