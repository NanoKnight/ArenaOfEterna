// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/EqiupmentSlotWidget.h"
#include"HUD/InventorySlotWidget.h"
#include"../WarriorCharacter.h"
#include"./Components/InventorySystem/InventoryComponent.h"
#include "Blueprint/DragDropOperation.h"
#include "HUD/InventoryWidget.h"
#include"Components\TextBlock.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include"Components\Image.h"


bool UEqiupmentSlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	UInventorySlotWidget* DraggedITem = Cast<UInventorySlotWidget>(InOperation->Payload);
	if (DraggedITem && DraggedITem->Item.ItemTypes == ItemTypes)
	{
		AWarriorCharacter* Warrior = Cast<AWarriorCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if (Warrior)
		{
			if (DraggedITem)
			{
				if (!EquippedItem.ItemName.IsEmpty())
				{

					FInventoryStruct TempItem = DraggedITem->Item;
					FInventoryStruct TempEqItem = EquippedItem;


					Warrior->GetInventoryComponent()->UnEquipItem(EquippedItem, DraggedITem->EquippedItemActor);


					//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("dolu")));
					

					Warrior->GetInventoryComponent()->EquipItem(TempItem);
					DraggedITem->ItemIcon->SetBrushFromSoftTexture(TempEqItem.ItemIcon);
					DraggedITem->ItemName->SetText(FText::FromString(TempEqItem.ItemName));
					EquippedItem = TempItem;
					

					//Warrior->GetInventoryComponent()->AddItem(TempEqItem);
					if (TempItem.ItemIcon)
					{
						EquipmentIcon->SetBrushFromSoftTexture(TempItem.ItemIcon);

					}
					//Warrior->GetInventoryComponent()->InventoryWidget->UpdateInventoryDisplay(Warrior->GetInventoryComponent()->InventoryItems);


					int32 OldItemIndex = Warrior->GetInventoryComponent()->FindItemIndex(EquippedItem);
					if (OldItemIndex != INDEX_NONE)
					{
						Warrior->GetInventoryComponent()->AddItemWithIndex(TempEqItem ,OldItemIndex);
						Warrior->GetInventoryComponent()->RemoveFormInventory(DraggedITem->Item);
						Warrior->GetInventoryComponent()->InventoryWidget->UpdateInventoryDisplay(Warrior->GetInventoryComponent()->InventoryItems);

						
					}				
					

				}

				else
				{

					Warrior->EquipItem(DraggedITem->Item);
					EquippedItem = DraggedITem->Item;
					EquippedItemActor = DraggedITem->EquippedItemActor;
					EquipmentIcon->SetBrushFromSoftTexture(DraggedITem->Item.ItemIcon);
					Warrior->GetInventoryComponent()->SetDefaultInventoryValues(DraggedITem->Item,DraggedITem->ItemIndex);
					DraggedITem->ItemIcon->SetBrushFromSoftTexture(DraggedITem->ImageIconAsset);
					DraggedITem->Item = FInventoryStruct();
					DraggedITem->ItemName->SetText(FText::GetEmpty());
					
				}
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
	SetEquipmentSlotsNames();

	// SONRA KONTROL EDÝLMESÝ GEREKN BÝR YER
	AWarriorCharacter* WarriorChar = Cast<AWarriorCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (WarriorChar)
	{
		UE_LOG(LogTemp, Warning, TEXT("warrior succesfluy casted from equipment widget"));
		int32 EquipItemsCount = WarriorChar->GetInventoryComponent()->EquippedItems.Num();
		for (int32 i = 0; i < EquipItemsCount; i++)
		{
			FInventoryStruct& WarEquipItems = WarriorChar->GetInventoryComponent()->EquippedItems[i];
			if (WarEquipItems.ItemTypes == ItemTypes)
			{
				ItemTypes = WarEquipItems.ItemTypes;
				EquippedItem = WarEquipItems;
				EquipmentIcon->SetBrushFromTexture(WarEquipItems.ItemIcon);
				EquippedItemActor = WarEquipItems.ItemClass.GetDefaultObject();
			    
			}
		}
	}



}

void UEqiupmentSlotWidget::SetEquipmentSlotsNames()
{
	if (!SlotName)return;
	
		switch (ItemTypes)
		{
		case EItemTypes::Weapon:
			SlotName->SetText(FText::FromString(TEXT("Weapon")));
			break;

		case EItemTypes::Chest:
			SlotName->SetText(FText::FromString(TEXT("Chest Armor")));
			break;

		case EItemTypes::Feet:
			SlotName->SetText(FText::FromString(TEXT("Feet Armor")));
			break;

		case EItemTypes::Gloves:
			SlotName->SetText(FText::FromString(TEXT("Gloves Armor")));
			break;

		case EItemTypes::Head:
			SlotName->SetText(FText::FromString(TEXT("Head Armor")));
			break;

		case EItemTypes::Shield:
			SlotName->SetText(FText::FromString(TEXT("Shield")));
			break;

		case EItemTypes::Trousers:
			SlotName->SetText(FText::FromString(TEXT("Trousers Armor")));
			break;

		default:
			SlotName->SetText(FText::FromString(TEXT("")));
			break;

		}

	
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
	if (!EquippedItem.ItemName.IsEmpty())
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

}
