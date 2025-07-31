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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	int32 ItemIndex;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> ImageIconAsset;

	FInventoryStruct Item;

	ABaseItem* EquippedItemActor;

	UPROPERTY()
	int32 SlotIndex;

	void SetUp(const FInventoryStruct& NewItem);
	UFUNCTION()
	void SetSlotIndex(int32 NewIndex);
	UFUNCTION()
	void OnItemClicked();


	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent , UDragDropOperation*& OutOperation)override;
	virtual bool NativeOnDrop(const FGeometry& Geometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;

	int32 GetSlotIndex() const { return SlotIndex; }
	FInventoryStruct GetItem() const { return Item; }

	bool IfInventorySlotItemIsValid();


private:
	
};
