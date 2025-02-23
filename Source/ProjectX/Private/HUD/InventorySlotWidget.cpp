// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/InventorySlotWidget.h"
#include "Components/Image.h"
#include"../WarriorCharacter.h"
#include"Components\Button.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/DragDropOperation.h"
#include"Items\BaseItem.h"
#include "Components/TextBlock.h"

void UInventorySlotWidget::SetUp(const FInventoryStruct& NewItem)
{

	Item = NewItem;
	/*if (ItemButton)
	{
		//ItemButton->OnClicked.AddDynamic(this, &UInventorySlotWidget::OnItemClicked);
	}
	*/

	if (ItemIcon && Item.ItemIcon)
	{
		ItemIcon->SetBrushFromTexture(Item.ItemIcon);
		
	}

	if (ItemName)
	{
		ItemName->SetText(FText::FromString(Item.ItemName));
	}



	
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



	UDragDropOperation* DragDropOp = UWidgetBlueprintLibrary::CreateDragDropOperation(UDragDropOperation::StaticClass());
	if (DragDropOp)
	{
		DragDropOp->Payload = this;
		DragDropOp->DefaultDragVisual = this;
		DragDropOp->Pivot = EDragPivot::MouseDown;
		OutOperation = DragDropOp;

	}
}
