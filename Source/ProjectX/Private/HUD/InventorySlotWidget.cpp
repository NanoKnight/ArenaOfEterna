// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/InventorySlotWidget.h"
#include "Components/Image.h"
#include"../WarriorCharacter.h"
#include"Components\Button.h"
#include"Components\Border.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/DragDropOperation.h"
#include"HUD\EqiupmentSlotWidget.h"
#include"Items\BaseItem.h"
#include"HUD\InventoryWidget.h"
#include"Components\AttributeComponent.h"
#include"Components\InventorySystem\InventoryComponent.h"
#include "Components/TextBlock.h"

void UInventorySlotWidget::NativeConstruct()
{


	if (UseBtn)
	{
		UseBtn->OnClicked.AddDynamic(this, &UInventorySlotWidget::OnEquipClicked);
	}
	
	

	UseBtn->SetVisibility(ESlateVisibility::Collapsed);
	if (Item.ItemTypes == EItemTypes::Pot)
	{
		StackNumber->SetVisibility(ESlateVisibility::Visible);
	}
}


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

	if (StackNumber)
	{
		if (Item.ItemTypes == EItemTypes::Pot)
		{
			StackNumber->SetText(FText::FromString(FString::Printf(TEXT("%d"), Item.StackCounter)));
		}
	}
}

void UInventorySlotWidget::SetSlotIndex(int32 NewIndex)
{
	SlotIndex = NewIndex;
}

void UInventorySlotWidget::OnEquipClicked()
{
	AWarriorCharacter* Warrior = Cast<AWarriorCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (Warrior && Warrior->GetInventoryComponent() && Item.ItemTypes != EItemTypes::Pot)return;

	for (FInventoryStruct& WItem : Warrior->GetInventoryComponent()->InventoryItems)
	{
		if (WItem.StackCounter > 0)
		{
			if (StackNumber && WItem.ItemTypes == EItemTypes::Pot)
			{
				WItem.StackCounter -= 1;
				
				StackNumber->SetText(FText::FromString(FString::Printf(TEXT("%d"),WItem.StackCounter)));
				Warrior->GetAttributesComponent()->AddHealth(Warrior->GetAttributesComponent()->GetPotHealth());
				Warrior->InitializePlayerOverlay();
			}
		

		}
		if(WItem.StackCounter <= 0 && WItem.ItemTypes == EItemTypes::Pot)
		{
			//Warrior->GetInventoryComponent()->RemoveFormInventory(WItem);
			UseBtn->SetVisibility(ESlateVisibility::Collapsed);
			StackNumber->SetVisibility(ESlateVisibility::Collapsed);
			Warrior->GetInventoryComponent()->SetDefaultItemValue(WItem);
			ItemIcon->SetBrushFromSoftTexture(ImageIconAsset);
			Item = FInventoryStruct();
			ItemName->SetText(FText::GetEmpty());
		}
	}
	
		
}



FReply UInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);


	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
        
		if (Item.ItemTypes == EItemTypes::Pot)
		{
			UseBtn->SetVisibility(ESlateVisibility::Visible);
		}
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

		UseBtn->SetVisibility(ESlateVisibility::Collapsed);
		DragDropOp->Payload = this;
		DragDropOp->DefaultDragVisual = this;
		DragDropOp->Pivot = EDragPivot::TopLeft;
		OutOperation = DragDropOp;
	}
}

bool UInventorySlotWidget::NativeOnDrop(const FGeometry& Geometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
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
				 int32 OldIndex = Warrior->GetInventoryComponent()->FindItemIndex(Item);
				 Warrior->GetInventoryComponent()->UnEquipItem(DraggedItem->EquippedItem, DraggedItem->EquippedItemActor);
				 Warrior->GetInventoryComponent()->AddItemWithIndex(DraggedItem->EquippedItem,OldIndex);
				 Warrior->GetInventoryComponent()->RemoveFormInventory(Item);
				 Warrior->GetInventoryComponent()->InventoryWidget->UpdateInventoryDisplay(Warrior->GetInventoryComponent()->InventoryItems);


				 // 2. Yeni itemi kuþan
				 DraggedItem->EquipmentIcon->SetBrushFromSoftTexture(Item.ItemIcon);
				 DraggedItem->EquippedItem = Item;
				 DraggedItem->EquippedItemActor = EquippedItemActor;
				 Warrior->GetInventoryComponent()->EquipItem(DraggedItem->EquippedItem);
				 GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("eq item cikti")));

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

void UInventorySlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	if (UseBtn)
	{
		UseBtn->SetVisibility(ESlateVisibility::Collapsed);
	}
}

bool UInventorySlotWidget::IfInventorySlotItemIsValid()
{
	return !Item.ItemName.IsEmpty();
}

