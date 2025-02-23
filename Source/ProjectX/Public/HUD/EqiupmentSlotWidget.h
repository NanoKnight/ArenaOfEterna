// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include"../Structs/InventoryStruct.h"
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
	 
	 UPROPERTY(EditAnywhere)
	 EItemTypes ItemTypes;

	 UPROPERTY(EditAnywhere)
	 TObjectPtr<UTexture2D> EquipmentImage;

	 UPROPERTY(EditAnywhere,BlueprintReadWrite)
	 FInventoryStruct EquippedItem;

	 UPROPERTY(EditAnywhere, BlueprintReadWrite)
	 ABaseItem* EquippedItemActor;



	void SetItemIcon(UTexture2D* NewIcon);
	void SetDefaultWeaponIcon();


protected:
	virtual void NativeConstruct() override;


	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)override;
	
};
