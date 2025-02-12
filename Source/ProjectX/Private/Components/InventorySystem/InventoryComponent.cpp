// Fill out your copyright notice in the Description page of Project Settings.


#include"./Components/InventorySystem/InventoryComponent.h"
#include"../WarriorCharacter.h"
#include "./Items/Weapons/Weapon.h"


// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventoryComponent::AddItem(const FInventoryStruct& NewItem)
{
	InventoryItems.Add(NewItem);
}

void UInventoryComponent::EquipItem(const FInventoryStruct& ItemToEquip)
{
	if (EquippedItems.Contains(ItemToEquip.EquipmentSlot))
	{
		UnEquipItem(ItemToEquip.EquipmentSlot);
	}

	EquippedItems.Add(ItemToEquip.EquipmentSlot, ItemToEquip);

	AWarriorCharacter* WarriorCharacter = Cast<AWarriorCharacter>(GetOwner());
	if (WarriorCharacter)
	{
		UWorld* World = WarriorCharacter->GetWorld();
		if (World)
		{
			AWeapon* SpawnedWeapon = World->SpawnActor<AWeapon>(ItemToEquip.ItemClass);
			if (SpawnedWeapon)
			{
				SpawnedWeapon->Equip(WarriorCharacter->GetMesh(), "WeaponSocket", WarriorCharacter, WarriorCharacter);
				WarriorCharacter->EquippedWeapon = SpawnedWeapon;
				WarriorCharacter->EquipWeapon(WarriorCharacter->EquippedWeapon);
			}
		}
	}
}

void UInventoryComponent::UnEquipItem(EEquipmentSlot Slot)
{
	if (EquippedItems.Contains(Slot))
	{
		EquippedItems.Remove(Slot);
	}
}

