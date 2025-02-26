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

	virtual bool NativeOnDrop(const FGeometry& Geometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;

	void CheckHoveringOnInventoryList(const FDragDropEvent& InDragDropEvent);

	bool IsHoveringOnInventoryList();

	UFUNCTION(BlueprintCallable)
	void UpdateInventoryDisplay(const TArray<FInventoryStruct>& InventoryItems);


	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;
	



};
