// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h" 
#include "Engine/DataTable.h"
#include"InventoryStruct.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EEquipmentSlot : uint8
{
	Head UMETA(DisplayName ="Head"),
	Chest UMETA(DisplayName="Chest"),
	Legs UMETA(DisplayName = "Legs"),
	Feet UMETA(DisplayName = "Feet"),
	Weapon UMETA(DisplayName = "Weapon"),
	Shield UMETA(DisplayName="Shield")

};

USTRUCT(BlueprintType)
struct FInventoryStruct
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEquipmentSlot EquipmentSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor>ItemClass;



	FInventoryStruct();
	~FInventoryStruct();
};
