// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include"../../Structs/InventoryStruct.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTX_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Inventory")
	TArray<FInventoryStruct> InventoryItems;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TMap<EEquipmentSlot, FInventoryStruct>EquippedItems;

	UFUNCTION(BlueprintCallable,Category="Inventory")
	void AddItem(const FInventoryStruct& NewItem);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void EquipItem(const FInventoryStruct& ItemToEquip);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void UnEquipItem(EEquipmentSlot Slot);

};

