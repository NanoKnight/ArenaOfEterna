// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Structs/InventoryStruct.h"
#include "Framework/Application/SlateApplication.h"
#include "InventorySlotWidget.generated.h"

class UButton;
class UImage;
class UTextBlock;
class UBorder;
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

	UPROPERTY(meta = (BindWidget ,EditCondition = "ItemTypes == EItemTypes::Pot", EditConditionHides))
	UTextBlock* StackNumber;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Inventory")
	int32 ItemIndex;

	UPROPERTY(meta = (BindWidget))
	UButton* UseBtn;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> ImageIconAsset;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FInventoryStruct Item;

	UPROPERTY()
	ABaseItem* EquippedItemActor;

	UPROPERTY()
	int32 SlotIndex;

	void SetUp(const FInventoryStruct& NewItem);

	UFUNCTION()
	void SetSlotIndex(int32 NewIndex);

	UFUNCTION()
	void OnEquipClicked();







	

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent , UDragDropOperation*& OutOperation)override;
	virtual bool NativeOnDrop(const FGeometry& Geometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	int32 GetSlotIndex() const { return SlotIndex; }
	FInventoryStruct GetItem() const { return Item; }

	bool IfInventorySlotItemIsValid();

protected:
	virtual void NativeConstruct() override;

private:
	bool bIsDraging = false;
	float DragStartTime = 0;
	const float DragThreshold = 0.2f;
	
};
