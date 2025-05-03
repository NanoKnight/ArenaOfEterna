// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h" 
#include "Engine/DataTable.h"
#include"InventoryStruct.generated.h"

/**
 * 
 */
class ABaseItem;
UENUM(BlueprintType)
enum class EEquipmentSlot : uint8
{
	None UMETA(DisplayName = "None"),
	Head UMETA(DisplayName ="Head"),
	Chest UMETA(DisplayName="Chest"),
	Feet UMETA(DisplayName = "Feet"),
	Trousers UMETA(DisplayName = "trousers"),
	Gloves UMETA(DisplayName = "Gloves"),
	Weapon UMETA(DisplayName = "Weapon"),
	Shield UMETA(DisplayName="Shield")

};

UENUM(BlueprintType)
enum class EItemTypes : uint8
{
	None,
	Head,
	Chest,
	Feet,
	Trousers,
	Gloves,
	Weapon,
	Shield

};

USTRUCT(BlueprintType)
struct FInventoryStruct
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEquipmentSlot EquipmentSlot;

	UPROPERTY(EditAnywhere,BlueprintReadwrite)
	FName ItemSocketName;

	UPROPERTY(EditAnywhere)
	EItemTypes ItemTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABaseItem>ItemClass;

	bool operator==(const FInventoryStruct& Other) const
	{
		return ItemName == Other.ItemName && EquipmentSlot == Other.EquipmentSlot && ItemTypes == Other.ItemTypes;
	}

	FInventoryStruct();
	~FInventoryStruct();
};
