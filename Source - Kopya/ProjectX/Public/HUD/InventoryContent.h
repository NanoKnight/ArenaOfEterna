// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryContent.generated.h"

class UVerticalBox;

/**
 * 
 */
UCLASS()
class PROJECTX_API UInventoryContent : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* InventoryList;
	
};
