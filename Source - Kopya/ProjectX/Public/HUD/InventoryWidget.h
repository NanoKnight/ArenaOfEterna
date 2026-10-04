// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Structs/InventoryStruct.h"
#include "InputCoreTypes.h" 
#include "Components/Widget.h"
#include "InventoryWidget.generated.h"

class UGridPanel;
class UInventorySlotWidget;

/**
 * 
 */
UCLASS()
class PROJECTX_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	

	UPROPERTY(meta = (BindWidget))
	UGridPanel* InventoryList;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UInventorySlotWidget> InventorySlotWidgetClass;

	UPROPERTY(EditAnywhere)
	bool bIsHovering;

	UPROPERTY()
	TArray<UInventorySlotWidget*> InventorySlots;

	TArray<int32> StoredSlotIndices;
	
	void SetDefenseText(int32 DefensePoint);

	void CheckHoveringOnInventoryList(const FDragDropEvent& InDragDropEvent);

	bool IsHoveringOnInventoryList();

	void SwapItems(int32 FromIndex, int32 ToIndex);

	void SaveSlotIndices();

	UFUNCTION(BlueprintCallable)
	void UpdateInventoryDisplay(const TArray<FInventoryStruct>& InventoryItems);

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	private:
		UPROPERTY()
		int32 SlotIndex;

		UPROPERTY(meta = (BindWidget))
		class UTextBlock* DefensePointText;



protected:
	virtual void NativeConstruct() override;




};
