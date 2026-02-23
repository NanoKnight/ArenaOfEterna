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
	None UMETA(DisplayName = "None"),
	Pot UMETA(DisplayName = "Pot"),
	Head UMETA(DisplayName = "Head"),
	Chest UMETA(DisplayName = "Chest"),
	Feet UMETA(DisplayName = "Feet"),
	Trousers UMETA(DisplayName = "Trousers"),
	Gloves UMETA(DisplayName = "Gloves"),
	Weapon UMETA(DisplayName = "Weapon"),
	Shield UMETA(DisplayName = "Shield")
};

USTRUCT(BlueprintType)
struct FInventoryStruct
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* ItemStaticMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* ItemIcon;
	
	UPROPERTY(EditAnywhere, meta = (EditCondition = "ItemTypes != EItemTypes::Pot", EditConditionHides))
	EEquipmentSlot EquipmentSlot;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "ItemTypes != EItemTypes::Pot", EditConditionHides))
	FName ItemSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemTypes ItemTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite , meta = (EditCondition = "ItemTypes == EItemTypes::Weapon", EditConditionHides))
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta = (EditCondition = "ItemTypes != EItemTypes::Weapon && ItemType != EItemTypes::Pot", EditConditionHides))
	float Defense;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "ItemTypes == EItemTypes::Pot", EditConditionHides))
	int32 StackCounter = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABaseItem>ItemClass;

	bool operator==(const FInventoryStruct& Other) const
	{
		return ItemName == Other.ItemName && EquipmentSlot == Other.EquipmentSlot && ItemTypes == Other.ItemTypes;
	}

	FInventoryStruct();
	~FInventoryStruct();
};
