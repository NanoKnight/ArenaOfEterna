// Fill out your copyright notice in the Description page of Project Settings.


#include"./Components/InventorySystem/InventoryComponent.h"
#include"../WarriorCharacter.h"
#include"Components\AttributeComponent.h"
#include "HUD/InventoryWidget.h"
#include "HUD\EqiupmentSlotWidget.h"
#include "Components/GridPanel.h"
#include"Components\GridSlot.h"
#include"Items\BaseItem.h"
#include"HUD/CharacterHUD.h"
#include"HUD\PlayerHUD.h"
#include "TimerManager.h"
#include"Characters\PreviewCharacter.h"
#include "Kismet/GameplayStatics.h"
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
	MainCharacter = Cast<AWarriorCharacter>(GetOwner());
	ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(GetOwner());


	
	
	if (!GetOwner()->ActorHasTag("Enemy"))
	{
    	InventoryItems.SetNum(40);


		for (int32 i = 0; i < InventoryItems.Num(); i++)
		{

			FInventoryStruct& Item = InventoryItems[i];

			if (Item.ItemName.IsEmpty() && Item.ItemClass == nullptr)
			{
				Item.ItemName = FString("");
				Item.ItemIcon = nullptr;
				Item.ItemClass = nullptr;
				Item.ItemStaticMesh = nullptr;
				Item.ItemSocketName = NAME_None;
				Item.ItemTypes = EItemTypes::None;
				Item.EquipmentSlot = EEquipmentSlot::None;
			}



			if (Item.ItemName.IsEmpty() && Item.ItemClass != nullptr)
			{

				if (TSubclassOf<ABaseItem> BaseItemClass = Item.ItemClass)
				{
					if (ABaseItem* BaseItem = BaseItemClass.GetDefaultObject())
					{

						Item.ItemName = BaseItem->ItemName;
						Item.ItemIcon = BaseItem->ItemIcon;
						Item.ItemStaticMesh = BaseItem->GetItemMesh()->GetStaticMesh();
						Item.ItemSocketName = BaseItem->ItemSocketName;
						Item.ItemTypes = BaseItem->ItemType;
						Item.EquipmentSlot = BaseItem->ItemEquipmentSlot;
						Item.Damage = BaseItem->Damage;
						Item.Defense = BaseItem->Defense;	
						
					}
				}
			}
		}


	}

	

}


	

void UInventoryComponent::SetDefensePoint()
{
	if (InventoryWidget)
	{
		InventoryWidget->SetDefenseText(MainCharacter->GetAttributesComponent()->GetDefense());

	}
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
			//i hangi slota yerleþeceðini gösteriyor
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
			if (HUDRef)
			{
				UCharacterHUD* PlayerOverlay = HUDRef->GetPlayerOverlay();
				if (PlayerOverlay)
				{
					PlayerOverlay->SetReceivedItemText(NewItem.ItemName);
					PlayerOverlay->PlayItemReceivedTextAnimationFadeIn();
					GetOwner()->GetWorld()->GetTimerManager().SetTimer(ItemTextAnimTimer, FTimerDelegate::CreateUObject(this, &UInventoryComponent::PlayItemTextFadeOutAnim, PlayerOverlay), 1.f, false);
				}
			}
					
		}
	}


	if (InventoryWidget)
	{
		InventoryWidget->UpdateInventoryDisplay(InventoryItems);

	}

}

void UInventoryComponent::AddItemWithIndex(const FInventoryStruct& NewItem, int32 Index)
{


	//InventoryItems.Add(NewItem);

	for (int32 i = 0; i < InventoryItems.Num(); i++)
	{

		if (InventoryItems[i].ItemName.IsEmpty())
		{
			//i hangi slota yerleþeceðini gösteriyor
			InventoryItems[Index] = NewItem;
			break;
		}
	}
}




void UInventoryComponent::InventoryFullText()
{
	AWarriorCharacter* Warrior = Cast<AWarriorCharacter>(GetOwner());
	if (Warrior)
	{
		APlayerController* PlayerController = Cast<APlayerController>(Warrior->GetController());
		if (PlayerController)
		{
			APlayerHUD* HUDRef = Cast<APlayerHUD>(PlayerController->GetHUD());
			UCharacterHUD* PlayerOverlay = HUDRef->GetPlayerOverlay();
			if (PlayerOverlay && PlayerOverlay->NotifyTextVisiblity == ESlateVisibility::Hidden)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("cagirildi")));
				PlayerOverlay->PlayItemReceivedTextAnimationFadeIn();
				GetOwner()->GetWorld()->GetTimerManager().SetTimer(ItemTextAnimTimer, FTimerDelegate::CreateUObject(this, &UInventoryComponent::PlayItemTextFadeOutAnim, PlayerOverlay), 1.f, false);
				PlayerOverlay->SetReceivedItemText("Inventory Is Full");

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
	if (ItemToEquip.ItemClass)
	{
	    
		bool itemExists = false;
		for (const FInventoryStruct& EquippedItem : EquippedItems)
		{
			if ( EquippedItem.ItemName == ItemToEquip.ItemName)
			{
				itemExists = true;
				break;

			}
		}
		if (itemExists == false)
		{
			//EquippedItems.Add(ItemToEquip);

		}
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
						SpawnedItem->GetItemMesh()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
						SpawnedItem->GetItemMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
						SpawnedItem->GetItemMesh()->SetSimulatePhysics(false);
						AWeapon* WeaponRef = Cast<AWeapon>(SpawnedItem);
						WarriorCharacter->EquipWeapon(WeaponRef);
						SpawnedItem->Equip(WarriorCharacter->GetMesh(), ItemToEquip.ItemSocketName, WarriorCharacter, WarriorCharacter);
						
						SpawnedItem->GetItemMesh()->SetStaticMesh(ItemToEquip.ItemStaticMesh);
						SpawnedItem->Damage = ItemToEquip.Damage;

					
					}


				}
				else
				{
					

					ABaseItem* SpawnedItem = World->SpawnActor<ABaseItem>(ItemToEquip.ItemClass);

					if (SpawnedItem)
					{
						SpawnedItem->Equip(WarriorCharacter->GetMesh(), ItemToEquip.ItemSocketName, WarriorCharacter, WarriorCharacter);
						SpawnedItem->GetItemMesh()->SetStaticMesh(ItemToEquip.ItemStaticMesh);
						SpawnedItem->Defense = ItemToEquip.Defense;
						WarriorCharacter->ItemsToEquip.Add(SpawnedItem);


					}

				

					if (SpawnedItem->Defense > 0 )
					{
						WarriorCharacter->GetAttributesComponent()->SetDefense(WarriorCharacter->GetAttributesComponent()->GetDefense() + SpawnedItem->Defense);
						SetDefensePoint();

					}
				}
			}
		}
	}
	
}




void UInventoryComponent::DropItem(const FInventoryStruct& ItemToDrop)
{
	AWarriorCharacter* WarriorCharacter = Cast<AWarriorCharacter>(GetOwner());
	if (WarriorCharacter)
	{
		UWorld* World = WarriorCharacter->GetWorld();
		if (World)
		{
			ABaseItem* SpawnedItem = GetWorld()->SpawnActor<ABaseItem>(ItemToDrop.ItemClass);
			if (SpawnedItem)
			{
				//SpawnedItem->GetItemSocketName() = ItemToDrop.ItemSocketName;
				//SpawnedItem->GetItemType() = ItemToDrop.ItemTypes;

				ABaseItem* ItemRef = Cast<ABaseItem>(SpawnedItem);
				if (ItemRef)
				{
					FVector Location = WarriorCharacter->GetActorLocation();
					Location.X = Location.X + -150.f;
					Location.Z = Location.Z + 30.f;
					SpawnedItem->SetActorLocation(Location);
					SpawnedItem->ItemSocketName = ItemToDrop.ItemSocketName;
					SpawnedItem->ItemName = ItemToDrop.ItemName;
					SpawnedItem->ItemIcon = ItemToDrop.ItemIcon;
					SpawnedItem->ItemType = ItemToDrop.ItemTypes;
					SpawnedItem->GetItemMesh()->SetSimulatePhysics(true);
					SpawnedItem->GetItemMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
					RemoveFormInventory(ItemToDrop);
				}

			
			}
		}
	}
}

int32 UInventoryComponent::FindFirstEmptySlot() const
{
	for (int32 i = 0; i < InventoryItems.Num(); ++i)
	{
		if (InventoryItems[i].ItemName.IsEmpty())
		{
			return i;
		}
	}
	return -1; // boþ slot yoksa
}

int32 UInventoryComponent::FindItemIndex(const FInventoryStruct& ItemToFind) const
{

	for (int32 i = 0; i < InventoryItems.Num(); i++)
	{
	    if (InventoryItems[i].ItemName == ItemToFind.ItemName)
	    {
			return i;
	    }
	}
	return INDEX_NONE;
}

void UInventoryComponent::UnEquipItem(FInventoryStruct& Item, ABaseItem* EquippedItem)
{
	if (EquippedItems.Contains(Item))
	{ 
		float defins = Item.Defense;
		EquippedItems.Remove(Item);

		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString(TEXT("Deleted")));

		AWarriorCharacter* WarriorCharacter = Cast<AWarriorCharacter>(GetOwner());



		if (WarriorCharacter)
		{
			
				WarriorCharacter->GetAttributesComponent()->SetDefense(WarriorCharacter->GetAttributesComponent()->GetDefense() - defins);
				SetDefensePoint();

			
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
						if (WarriorCharacter->ItemsToEquip[i] && WarriorCharacter->ItemsToEquip[i]->ItemType == Item.ItemTypes) 
						{
							
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
	
	InventoryItems.RemoveSingle(Item);
	if (InventoryWidget)
	{
		InventoryWidget->UpdateInventoryDisplay(InventoryItems);

	}


}

void UInventoryComponent::SetDefaultItemValueWithIndex(FInventoryStruct& Item, int32 Index)
{

	if (InventoryItems.IsValidIndex(Index))
	{
		InventoryItems[Index] = FInventoryStruct();
		InventoryItems[Index].ItemIcon = nullptr;
		InventoryItems[Index].ItemName = FString("");
		InventoryItems[Index].ItemClass = nullptr;
		InventoryItems[Index].Damage = 31;
		InventoryItems[Index].ItemSocketName = FName("");
		InventoryItems[Index].ItemTypes = EItemTypes::None;
		InventoryItems[Index].Defense = 31;
		InventoryItems[Index].ItemStaticMesh = nullptr;
		InventoryItems[Index].EquipmentSlot = EEquipmentSlot::None;
	};

}


void UInventoryComponent::SetDefaultItemValue(FInventoryStruct& Item)
{

	if (!Item.ItemName.IsEmpty())
	{
		Item = FInventoryStruct();
		Item.ItemIcon = nullptr;
		Item.ItemName = FString("");
		Item.ItemClass = nullptr;
		Item.Damage = 0;
		Item.ItemSocketName = FName("");
		Item.ItemTypes = EItemTypes::None;
		Item.Defense = 0;
		Item.ItemStaticMesh = nullptr;
		Item.EquipmentSlot = EEquipmentSlot::None;
	};
}
void UInventoryComponent::OpenInventory()
{
	if (GetOwner()->ActorHasTag("WarriorCharacter")) 
	{
	  CharacterLoc = GetOwner()->GetActorLocation();
	  CharacterRot = GetOwner()->GetActorRotation();
	}

	UWorld* World = GetOwner()->GetWorld();
	if (World)
	{
		
			
		
		APlayerController* Controller = World->GetFirstPlayerController();
		if (InventoryWidgetClass && Controller)
		{
			
			//Controller->Pause();
			CreateInventoryWidget(Controller);
			ToggleInventory(Controller);
			SetDefensePoint();

			
		}
	}
}
void UInventoryComponent::ToggleInventory(APlayerController* Controller)
{
	if (InventoryWidget)
	{

		AWarriorCharacter* WarriorCharacter = Cast<AWarriorCharacter>(GetOwner());

		if (InventoryWidget->GetVisibility() == ESlateVisibility::Visible)
		{
			// KAPATIRKEN verileri kaydet
			
			InventoryWidget->SaveSlotIndices();
			SavedSlotIndices = InventoryWidget->StoredSlotIndices;
			InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
			Controller->bShowMouseCursor = false;
			FInputModeGameOnly InputMode;
			WarriorCharacter->SetActorLocation(CharacterLoc);
			WarriorCharacter->SetActorRotation(CharacterRot);
			Controller->SetInputMode(InputMode);
		
		}
		else
		{
			// AÇARKEN kayýtlý verileri yükle
		

			if (GetOwner()->ActorHasTag("WarriorCharacter"))
			{
				GetOwner()->SetActorLocation(FVector(11500.000000,-2200.000000, 1160.000000));
				GetOwner()->SetActorRotation(FRotator(0, 90, 0));

				
			}
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
		return;
	}

	if (InventoryItems[ToIndex].ItemName.IsEmpty()) // Eðer hedef boþsa direkt taþý
	{
		InventoryItems[ToIndex] = InventoryItems[FromIndex];
		InventoryItems[FromIndex] = FInventoryStruct();
		InventoryItems[FromIndex].ItemName = FString("");
		InventoryItems[FromIndex].ItemClass = nullptr;
		InventoryItems[FromIndex].EquipmentSlot = EEquipmentSlot::None;
		InventoryItems[FromIndex].ItemSocketName = NAME_None;
		InventoryItems[FromIndex].ItemTypes = EItemTypes::None;
		InventoryItems[FromIndex].ItemIcon = nullptr;

	}
	else
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




