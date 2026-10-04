// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include"../Structs/InventoryStruct.h"
#include "ItemInteractionWidget.generated.h"


class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECTX_API UItemInteractionWidget : public UUserWidget
{
	GENERATED_BODY()

	public : 

		void SetInteractionValues(FString ItemName, int32 ItemAbility , EItemTypes ItemTypes);



	protected:



	private:

		UPROPERTY(meta = (BindWidget))
		UTextBlock* ItemNameText;

		UPROPERTY(meta = (BindWidget))
		UTextBlock* ItemAbilityText;





	
};
