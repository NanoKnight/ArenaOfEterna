// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include"../../Structs/InventoryStruct.h"
#include"../../Characters/PreviewCharacter.h"
#include "InventoryComponent.generated.h"

class UInventoryWidget;
class AWarriorCharacter;
class APlayerHUD;
class UCharacterHUD;
class APreviewCharacter;

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

	void SetDefensePoint();

	UPROPERTY();
	FVector CharacterLoc;

	UPROPERTY();
	FRotator CharacterRot;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Inventory")
	TArray<FInventoryStruct> InventoryItems;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<FInventoryStruct>EquippedItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<int32> SavedSlotIndices;
	UPROPERTY()
	int32 OldSlotIndex;

	UFUNCTION(BlueprintCallable,Category="Inventory")
	void AddItem(const FInventoryStruct& NewItem);

	void AddItemWithIndex(const FInventoryStruct& NewItem, int32 Index);


	void PlayItemTextFadeOutAnim(UCharacterHUD* PlayerOverlay);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void EquipItem(const FInventoryStruct& ItemToEquip);



	UFUNCTION(BlueprintCallable,Category = "Inventory")
	void DropItem(const FInventoryStruct& ItemToDrop);

	int32 FindFirstEmptySlot() const;

	int32 FindItemIndex(const FInventoryStruct& ItemToFind) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void UnEquipItem(FInventoryStruct& Item , ABaseItem* EquippedItem);

	UFUNCTION(BlueprintCallable,Category="Inventory")
	void RemoveFormInventory(const FInventoryStruct& Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetDefaultItemValueWithIndex(FInventoryStruct& Item , int32 Index);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetDefaultItemValue(FInventoryStruct& Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void OpenInventory();



	void ToggleInventory(APlayerController* Controller);
	
	void SwapInventoryItems(int32 FromIndex, int32 ToIndex);
	

	void InventoryFullText();

	UFUNCTION(BlueprintCallable)
	void MoveItem(int32 FromIndex, int32 ToIndex);

	UPROPERTY(EditAnywhere)
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;

	UInventoryWidget* InventoryWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<APreviewCharacter> PreviewClass;
private:
	void CreateInventoryWidget(APlayerController* Controller);

	FTimerHandle ItemTextAnimTimer;

	//APlayerController* PlayerController;
	AWarriorCharacter* MainCharacter;
	//APlayerHUD* PlayerHUD;
	//UCharacterHUD* PlayerOverlay;





};

