// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EqiupmentSlotWidget.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class PROJECTX_API UEqiupmentSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual bool NativeOnDrop(const FGeometry& InGemotry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;

	UPROPERTY(meta = (BindWidget))
	 UImage* EquipmentIcon;


	void SetItemIcon(UTexture2D* NewIcon);
	
};
