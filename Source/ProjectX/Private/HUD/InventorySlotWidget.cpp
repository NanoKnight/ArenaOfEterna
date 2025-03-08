// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/InventorySlotWidget.h"
#include "Components/Image.h"
#include"../WarriorCharacter.h"
#include"Components\Button.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/DragDropOperation.h"
#include"HUD\EqiupmentSlotWidget.h"
#include"Items\BaseItem.h"
#include"Components\InventorySystem\InventoryComponent.h"
#include "Components/TextBlock.h"

void UInventorySlotWidget::SetUp(const FInventoryStruct& NewItem)
{
	Item = NewItem;
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
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("bos")));

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
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("InOperation or Payload is null!"));
		return false;
	}

	UEqiupmentSlotWidget* DraggedItem = Cast<UEqiupmentSlotWidget>(InOperation->Payload);
	if (DraggedItem)
	{
		AWarriorCharacter* Warrior = Cast<AWarriorCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		
		
		if (Warrior && DraggedItem && !DraggedItem->EquippedItem.ItemName.IsEmpty())
		{

			Warrior->GetInventoryComponent()->InventoryItems.Add(DraggedItem->EquippedItem);
			Item = DraggedItem->EquippedItem;
			SetUp(DraggedItem->EquippedItem);

			Warrior->GetInventoryComponent()->UnEquipItem(DraggedItem->EquippedItem, DraggedItem->EquippedItemActor);
			if (ItemIcon)
			{
				    //ItemIcon->SetBrushFromTexture(DraggedItem->EquippedItem.ItemIcon);
					DraggedItem->SetDefaultWeaponIcon();
			}
			
		}
		return true;


	}
	


	UInventorySlotWidget* DraggedOnSelf = Cast <UInventorySlotWidget>(InOperation->Payload);
	AWarriorCharacter* Warrior = Cast<AWarriorCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (Warrior && DraggedOnSelf && !DraggedOnSelf->Item.ItemName.IsEmpty() && !IfInventorySlotItemIsValid())
	{
		if (DraggedOnSelf && DraggedOnSelf == this)
		{
			//return false;
		}
		
		Warrior->GetInventoryComponent()->UnEquipItem(DraggedOnSelf->Item, DraggedOnSelf->EquippedItemActor);
		Item = DraggedOnSelf->Item;
		SetUp(DraggedOnSelf->Item);
		DraggedOnSelf->ItemName->SetText(FText::GetEmpty());

		
		if (ItemIcon)
		{
			DraggedOnSelf->ItemIcon->SetBrushFromTexture(ImageIconAsset);
		}
		DraggedOnSelf->Item = FInventoryStruct();

	
	

	

	}
	else if (IfInventorySlotItemIsValid())
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString(FString::Printf(TEXT("InventorySlotItemISValid"))));
	    //ýtem ismi buglý çalýþýyor
		FInventoryStruct TempItem = DraggedOnSelf->Item;

		DraggedOnSelf->ItemName->SetText(FText::FromString(Item.ItemName));
		DraggedOnSelf->ItemIcon->SetBrushFromTexture(Item.ItemIcon);
		DraggedOnSelf->Item = Item;
		//ItemName->SetText(FText::FromString(DraggedOnSelf->Item.ItemName));
	
		Item = TempItem;

		ItemName->SetText(FText::FromString(DraggedOnSelf->Item.ItemName));
		ItemIcon->SetBrushFromTexture(DraggedOnSelf->Item.ItemIcon);
		ItemIcon->SetBrushFromTexture(TempItem.ItemIcon);
		ItemName->SetText(FText::FromString(TempItem.ItemName));
		//Item = DraggedOnSelf->Item;

		
		//DraggedOnSelf->Item = FInventoryStruct();

		
		return true;
	}

	return false;
}

bool UInventorySlotWidget::IfInventorySlotItemIsValid()
{
	return !Item.ItemName.IsEmpty();
}
