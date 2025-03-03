// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Structs/InventoryStruct.h"
#include "InventorySlotWidget.generated.h"

class UButton;
class UImage;
class UTextBlock;
class AWarriorCharacter;
class ABaseItem;
/**
 * 
 */
UCLASS()
class PROJECTX_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UImage* ItemIcon;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemName;

	FInventoryStruct Item;

	ABaseItem* EquippedItemActor;

	//UPROPERTY(meta = (BindWidget))
	//UButton* ItemButton;

	void SetUp(const FInventoryStruct& NewItem);
	UFUNCTION()
	void OnItemClicked();

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent , UDragDropOperation*& OutOperation)override;

private:
	
};
