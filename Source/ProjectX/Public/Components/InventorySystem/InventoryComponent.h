// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include"../../Structs/InventoryStruct.h"
#include "InventoryComponent.generated.h"

class UInventoryWidget;
class AWarriorCharacter;
class APlayerHUD;
class UCharacterHUD;

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
	TArray<FInventoryStruct>EquippedItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<int32> SavedSlotIndices;

	UFUNCTION(BlueprintCallable,Category="Inventory")
	void AddItem(const FInventoryStruct& NewItem);

	void PlayItemTextFadeOutAnim(UCharacterHUD* PlayerOverlay);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void EquipItem(const FInventoryStruct& ItemToEquip);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void UnEquipItem(const FInventoryStruct& Item , ABaseItem* EquippedItem);

	UFUNCTION(BlueprintCallable,Category="Inventory")
	void RemoveFormInventory(const FInventoryStruct& Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void OpenInventory();

	void ToggleInventory(APlayerController* Controller);
	
	void SwapInventoryItems(int32 FromIndex, int32 ToIndex);

	UFUNCTION(BlueprintCallable)
	void MoveItem(int32 FromIndex, int32 ToIndex);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;

	UInventoryWidget* InventoryWidget;
private:
	void CreateInventoryWidget(APlayerController* Controller);

	FTimerHandle ItemTextAnimTimer;

	//APlayerController* PlayerController;
	//AWarriorCharacter* MainCharacter;
	//APlayerHUD* PlayerHUD;
	//UCharacterHUD* PlayerOverlay;





};

