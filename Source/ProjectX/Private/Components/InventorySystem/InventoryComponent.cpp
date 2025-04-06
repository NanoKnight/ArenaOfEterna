// Fill out your copyright notice in the Description page of Project Settings.


#include"./Components/InventorySystem/InventoryComponent.h"
#include"../WarriorCharacter.h"
#include "HUD/InventoryWidget.h"
#include "Components/GridPanel.h"
#include"Components\GridSlot.h"
#include"Items\BaseItem.h"
#include"HUD/CharacterHUD.h"
#include"HUD\PlayerHUD.h"
#include "TimerManager.h"
#include "./Items/Weapons/Weapon.h"


// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();	
	/******BUD DEÐÝÞKENNERÝ BÖYLE BEGÝNPLAYDE SETLEYÝP KULLANMAYI DENÝYCEÐÝZ BU SINIFIN H DOSYASINDAN BUNLARIN REFERANSLARINI AKTÝF ET KAPALILAR ÞUAN*******/
	/* MainCharacter = Cast<AWarriorCharacter>(GetOwner());
	 if (MainCharacter)
	 {
		 PlayerController = Cast<APlayerController>(MainCharacter->GetController());
		 if (PlayerController)
		 {
			 PlayerHUD = Cast<APlayerHUD>(PlayerController->GetHUD());
			 if (PlayerHUD)
			 {
				  PlayerOverlay = PlayerHUD->GetPlayerOverlay();


			 }


		 }

	 }*/

	InventoryItems.SetNum(20);
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UInventoryComponent::AddItem(const FInventoryStruct& NewItem)
{
	//InventoryItems.Add(NewItem);


	for (int32 i =0 ; i< InventoryItems.Num(); i++)
	{
		if (InventoryItems[i].ItemName.IsEmpty())
		{
			InventoryItems[i] = NewItem;
			break;
		}
	}

	AWarriorCharacter* Warrior = Cast<AWarriorCharacter>(GetOwner());
	if (Warrior)
	{
		APlayerController* PlayerController = Cast<APlayerController>(Warrior->GetController());
		if (PlayerController)
		{
			APlayerHUD* HUDRef = Cast<APlayerHUD>(PlayerController->GetHUD());
			UCharacterHUD* PlayerOverlay = HUDRef->GetPlayerOverlay();
			if (PlayerOverlay)
			{
				PlayerOverlay->SetReceivedItemText(NewItem.ItemName);
				PlayerOverlay->PlayItemReceivedTextAnimationFadeIn();
				GetOwner()->GetWorld()->GetTimerManager().SetTimer(ItemTextAnimTimer, FTimerDelegate::CreateUObject(this,&UInventoryComponent::PlayItemTextFadeOutAnim,PlayerOverlay),1.f,false);
				PlayerOverlay->SetReceivedItemTextVisibility(ESlateVisibility::Hidden);

			}		

		}
	}

}

void UInventoryComponent::PlayItemTextFadeOutAnim(UCharacterHUD* PlayerOverlay)
{
	PlayerOverlay->PlayItemReceivedTextAnimationFadeOut();
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
	//InventoryWidget->UpdateInventoryDisplay(InventoryItems);

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
			// KAPATIRKEN verileri kaydet
			InventoryWidget->SaveSlotIndices();
			SavedSlotIndices = InventoryWidget->StoredSlotIndices;

			InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
			Controller->bShowMouseCursor = false;
			FInputModeGameOnly InputMode;
			Controller->SetInputMode(InputMode);
		}
		else
		{
			// AÇARKEN kayýtlý verileri yükle
			InventoryWidget->StoredSlotIndices = SavedSlotIndices;
			InventoryWidget->UpdateInventoryDisplay(InventoryItems);

			InventoryWidget->SetVisibility(ESlateVisibility::Visible);
			Controller->bShowMouseCursor = true;
			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(InventoryWidget->TakeWidget());
			Controller->SetInputMode(InputMode);
		}
	}
}

void UInventoryComponent::SwapInventoryItems(int32 FromIndex, int32 ToIndex)
{

	if (!InventoryItems.IsValidIndex(FromIndex) || !InventoryItems.IsValidIndex(ToIndex)) return;

	InventoryItems.Swap(FromIndex, ToIndex);

	// 2. Debug mesajý (INDEX'LERÝ KONTROL EDÝN)
	UE_LOG(LogTemp, Warning, TEXT("Swapped %d (%s) <-> %d (%s)"),
		FromIndex, *InventoryItems[FromIndex].ItemName,
		ToIndex, *InventoryItems[ToIndex].ItemName);

	// 3. Envanteri güncelle
	if (InventoryWidget)
	{
		InventoryWidget->UpdateInventoryDisplay(InventoryItems);
	}
}

void UInventoryComponent::MoveItem(int32 FromIndex, int32 ToIndex)
{

	if (!InventoryItems.IsValidIndex(FromIndex))
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid FromIndex: %d"), FromIndex);
		return;
	}

	// Eðer hedef slot yoksa, yeni slot aç
	if (!InventoryItems.IsValidIndex(ToIndex))
	{

	}

	if (InventoryItems[ToIndex].ItemName.IsEmpty()) // Eðer hedef boþsa direkt taþý
	{
		InventoryItems[ToIndex] = InventoryItems[FromIndex];
		InventoryItems[FromIndex] = FInventoryStruct();

	}
	else // Dolularýn yerini deðiþtir
	{
		InventoryItems.Swap(FromIndex, ToIndex);
	}

	if (InventoryWidget)
	{
	

		InventoryWidget->UpdateInventoryDisplay(InventoryItems);
	}

	UE_LOG(LogTemp, Warning, TEXT("Moved: %d -> %d, Inventory Size: %d"),
		FromIndex, ToIndex, InventoryItems.Num());

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
			InventoryWidget->InventoryList->ClearChildren();
			InventoryWidget->InventorySlots.Empty();
			InventoryWidget->UpdateInventoryDisplay(InventoryItems);
			InputMode.SetWidgetToFocus(InventoryWidget->TakeWidget());
			//InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
			Controller->SetInputMode(InputMode);
			GEngine->AddOnScreenDebugMessage(0, 2.f, FColor::Cyan, FString::Printf(TEXT("First Opened")));
			Controller->bShowMouseCursor = true;

		}
	}
}




