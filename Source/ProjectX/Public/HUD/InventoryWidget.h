// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Structs/InventoryStruct.h"
#include "InventoryWidget.generated.h"

class UVerticalBox;
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
	UVerticalBox* InventoryList;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UInventorySlotWidget> InventorySlotWidgetClass;


	

	UFUNCTION(BlueprintCallable)
	void UpdateInventoryDisplay(const TArray<FInventoryStruct>& InventoryItems);
	



};
