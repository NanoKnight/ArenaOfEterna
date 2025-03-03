// Fill out your copyright notice in the Description page of Project Settings.


#include"./Components/InventorySystem/InventoryComponent.h"
#include"../WarriorCharacter.h"
#include "HUD/InventoryWidget.h"
#include"Items\BaseItem.h"
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
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UInventoryComponent::AddItem(const FInventoryStruct& NewItem)
{
	InventoryItems.Add(NewItem);
}

void UInventoryComponent::EquipItem(const FInventoryStruct& ItemToEquip)
{
	/* bu fonksiyonun equip weapon kýsmý gözden geçirilmeli weaponeq yerine arrayden kullanmayý denemelisin  */

	EquippedItems.Add(ItemToEquip);

	AWarriorCharacter* WarriorCharacter = Cast<AWarriorCharacter>(GetOwner());
	if (WarriorCharacter)
	{
		UWorld* World = WarriorCharacter->GetWorld();
		if (World)
		{
			if (ItemToEquip.ItemTypes == EItemTypes::Weapon)
			{
				
				ABaseItem* SpawnedItem = GetWorld()->SpawnActor<ABaseItem>(ItemToEquip.ItemClass);
				if (SpawnedItem)
				{
				AWeapon* WeaponRef = Cast<AWeapon>(SpawnedItem);
				WarriorCharacter->EquipWeapon(WeaponRef);
				SpawnedItem->Equip(WarriorCharacter->GetMesh(), ItemToEquip.ItemSocketName, WarriorCharacter, WarriorCharacter);

				}
		
			}
			
				ABaseItem* SpawnedItem = World->SpawnActor<ABaseItem>(ItemToEquip.ItemClass);
				SpawnedItem->Equip(WarriorCharacter->GetMesh(), ItemToEquip.ItemSocketName, WarriorCharacter, WarriorCharacter);
				WarriorCharacter->ItemsToEquip.Add(SpawnedItem);
		}
	}
}

void UInventoryComponent::UnEquipItem(const FInventoryStruct& Item, ABaseItem* EquippedItem)
{
	if (EquippedItems.Contains(Item))
	{
		EquippedItems.Remove(Item);

		AWarriorCharacter* WarriorCharacter = Cast<AWarriorCharacter>(GetOwner());
		if (WarriorCharacter)
		{
			UWorld* World = WarriorCharacter->GetWorld();
			if (World)
			{
				if (Item.ItemTypes == EItemTypes::Weapon)
				{
					WarriorCharacter->SetCharacterStates(ECharacterStates::ECS_UnEquipped);
					WarriorCharacter->EquippedWeapon->Destroy();
				}
				
					for (int32 i = WarriorCharacter->ItemsToEquip.Num() - 1; i >= 0; i--)
					{
						if (WarriorCharacter->ItemsToEquip[i] && WarriorCharacter->ItemsToEquip[i]->ItemType == Item.ItemTypes) {
							WarriorCharacter->ItemsToEquip[i]->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
							WarriorCharacter->ItemsToEquip[i]->Destroy();
							WarriorCharacter->ItemsToEquip.RemoveAt(i);
						}
					}
				
				  
			}
		}
		
	}
}

void UInventoryComponent::RemoveFormInventory(const FInventoryStruct& Item)
{
	
	InventoryItems.Remove(Item);
	InventoryWidget->UpdateInventoryDisplay(InventoryItems);


}

void UInventoryComponent::OpenInventory()
{
	
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* Controller = World->GetFirstPlayerController();
		if (InventoryWidgetClass && Controller)
		{
			//Controller->Pause();
			CreateInventoryWidget(Controller);
			ToggleInventory(Controller);

		}
	}
}
void UInventoryComponent::ToggleInventory(APlayerController* Controller)
{
	if (InventoryWidget)
	{
		if (InventoryWidget->GetVisibility() == ESlateVisibility::Visible)
		{
			InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
			InventoryWidget->UpdateInventoryDisplay(InventoryItems);
			Controller->bShowMouseCursor = false;
			FInputModeGameOnly InputMode;
			Controller->SetInputMode(InputMode);

		}
		else
		{
			InventoryWidget->SetVisibility(ESlateVisibility::Visible);
			InventoryWidget->UpdateInventoryDisplay(InventoryItems);
			Controller->bShowMouseCursor = true;
			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(InventoryWidget->TakeWidget());
			Controller->SetInputMode(InputMode);
           



		}

	}
}



void UInventoryComponent::CreateInventoryWidget(APlayerController* Controller)
{
	if (!InventoryWidget)
	{
		if (Controller && InventoryWidgetClass)
		{
			FInputModeUIOnly InputMode;

			InventoryWidget = CreateWidget<UInventoryWidget>(Controller, InventoryWidgetClass);
			InventoryWidget->AddToViewport();
			InventoryWidget->UpdateInventoryDisplay(InventoryItems);

			InputMode.SetWidgetToFocus(InventoryWidget->TakeWidget());  
			//InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
			Controller->SetInputMode(InputMode);
			GEngine->AddOnScreenDebugMessage(0, 2.f, FColor::Cyan, FString::Printf(TEXT("First Opened")));
			Controller->bShowMouseCursor = true;

		}
	}
}



