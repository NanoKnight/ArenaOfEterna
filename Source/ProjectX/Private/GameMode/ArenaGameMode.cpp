// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/ArenaGameMode.h"
#include"Enemy\Enemy.h"
#include"Components\AttributeComponent.h"
#include"Components\InventorySystem\InventoryComponent.h"
#include "SaveGames/EternaSaveGame.h"
#include"Items\BaseItem.h"
#include "Kismet/GameplayStatics.h"
#include "./Items/EnemySpawner.h"
#include"Components\AudioComponent.h"
#include"../WarriorCharacter.h"
#include"Items\SpawnManager.h"
#include"Items/Weapons/Weapon.h"
#include"Runtime/Engine/Public/TimerManager.h"

AArenaGameMode::AArenaGameMode()
{
	
}

void AArenaGameMode::BeginPlay()
{
	Super::BeginPlay();
	LoadGame();
 
	/*
	if (EnemySpawner)
	{
		EnemySpawner = Cast<AEnemySpawner>(UGameplayStatics::GetActorOfClass(GetWorld(), AEnemySpawner::StaticClass()));
		NextWaveEnemyCount = EnemySpawner->EnemySpawnCount;
	}
	

	if (!SpawnManager)
	{
		
		TArray<AActor*> FoundSpawnManagers;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnManager::StaticClass(), FoundSpawnManagers);
		
		if (FoundSpawnManagers.Num() > 0)
		{
			SpawnManager = Cast<ASpawnManager>(FoundSpawnManagers[0]);
		}
	}
	if (SpawnManager)
	{
		SpawnManager->TriggerSpawnerByID(EnemySpawner->SpawnerID);
	}
		else if (EnemySpawner)
		{
			EnemySpawner->SpawnEnemy(NextWaveEnemyCount);
	}
	*/
}

void AArenaGameMode::RespawnEnemyStart_Implementation()
{
	if (EnemySpawner->WaveMode == false) return;
	WaveStarted = true;
	GetWorld()->GetTimerManager().SetTimer(WaveStartTimer, this, &AArenaGameMode::RespawnEnemy, 10, false);
	WaveCount = ++WaveCount;
}


void AArenaGameMode::RespawnEnemy()
{
	if (EnemySpawner->WaveMode == true)
	{
		WaveStarted = false;
		EnemySpawner->SpawnEnemy(NextWaveEnemyCount);
	}
}

void AArenaGameMode::SaveGame()
{
	AWarriorCharacter* WarriorCharacter = Cast<AWarriorCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	UEternaSaveGame* SaveGameObject = Cast <UEternaSaveGame>(UGameplayStatics::CreateSaveGameObject(UEternaSaveGame::StaticClass()));
	if (SaveGameObject && WarriorCharacter)
	{
		Attributes = WarriorCharacter->GetAttributesComponent();
		SaveGameObject->PlayerLocation = WarriorCharacter->GetActorLocation();
		SaveGameObject->CurrentQuestIndex = WarriorCharacter->CurrentQuestIndex;
		SaveGameObject->SavedInventoryItems = WarriorCharacter->GetInventoryComponent()->InventoryItems;
		SaveGameObject->SavedEquipItems = WarriorCharacter->GetInventoryComponent()->EquippedItems;
		SaveGameObject->Health = Attributes->GetHealth();
		SaveGameObject->Level = Attributes->GetLevel();
		SaveGameObject->Stamina = Attributes->GetStamina();
		SaveGameObject->Exp = Attributes->GetExperience();
		SaveGameObject->MaxExp = Attributes->GetMaxExperience();
		SaveGameObject->Gold = Attributes->GetGold();
		SaveGameObject->WaveCount = WaveCount;
		SaveGameObject->EnemyNextWaveCount = NextWaveEnemyCount;
		TArray<FString> KilledEnemiesList = KilledEnemiesNames;
		
		for (FString EnemyName : KilledEnemiesList)
		{
			SaveGameObject->AddKilledEnemy(EnemyName);
			

		}
		TArray<FString> AddedItemsArray = AddedItems;
		for (FString AddedItemsa : AddedItemsArray)
		{
			SaveGameObject->AddItems(AddedItemsa);
		}
		UGameplayStatics::SaveGameToSlot(SaveGameObject, TEXT("Save"), 0);
	}
}

void AArenaGameMode::LoadGame()
{
	UEternaSaveGame* SaveGameObject = Cast<UEternaSaveGame>(UGameplayStatics::LoadGameFromSlot("Save", 0));
	if (SaveGameObject)
	{
		AWarriorCharacter* WarriorCharacter = Cast<AWarriorCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		Attributes = WarriorCharacter->GetAttributesComponent();

		if (Attributes && WarriorCharacter)
		{
			Attributes->SetHealth(SaveGameObject->Health);
			Attributes->SetLevel(SaveGameObject->Level);
			Attributes->SetStamina(SaveGameObject->Stamina);
			Attributes->SetExp(SaveGameObject->Exp);
			Attributes->SetMaxExp(SaveGameObject->MaxExp);
			Attributes->SetGold(SaveGameObject->Gold);
			WarriorCharacter->SetActorLocation(SaveGameObject->PlayerLocation);
			WarriorCharacter->CurrentQuestIndex = SaveGameObject->CurrentQuestIndex;
			WarriorCharacter->GetInventoryComponent()->InventoryItems = SaveGameObject->SavedInventoryItems;
			WarriorCharacter->GetInventoryComponent()->EquippedItems = SaveGameObject->SavedEquipItems;
			WaveCount = SaveGameObject->WaveCount;
			NextWaveEnemyCount = SaveGameObject->EnemyNextWaveCount;
		}

		
		int32 EquipItemsCount = WarriorCharacter->GetInventoryComponent()->EquippedItems.Num();
		for (int32 i = 0; i < EquipItemsCount; i ++)
		{
			FInventoryStruct& EquipItems = WarriorCharacter->GetInventoryComponent()->EquippedItems[i];
			ABaseItem* SpawnedItem = GetWorld()->SpawnActor<ABaseItem>(EquipItems.ItemClass);
			if (SpawnedItem)
			{

				SpawnedItem->Equip(WarriorCharacter->GetMesh(), SpawnedItem->ItemSocketName, this, WarriorCharacter);
				if (SpawnedItem->ItemType == EItemTypes::Weapon)
				{
					AWeapon* BaseWeapon = Cast<AWeapon>(SpawnedItem);
					WarriorCharacter->EquippedWeapon = BaseWeapon;
					WarriorCharacter->SetCharacterStates(ECharacterStates::ECS_EquippedOnehand);
					BaseWeapon->Owner = WarriorCharacter;

				}

				else if (SpawnedItem->Defense > 0)
				{
					Attributes->SetDefense(Attributes->GetDefense() + SpawnedItem->Defense);
				}
			}

		}

		TArray<FString> LoadedEnemies = SaveGameObject->KilledEnemiesName;
		KilledEnemiesNames = LoadedEnemies;

		for (FString EnemyName : KilledEnemiesNames)
		{
			RemoveEnemyFromWorld();

		}

		TArray<FString> LoadaedItems = SaveGameObject->AddedItems;
		AddedItems = LoadaedItems;
		for (FString AddedItemsArray : AddedItems)
		{
			RemoveItemFormWorld();
			//WarriorCharacter->WeaponClass = SaveGameObject->EquippedWeapon;

		}
	}
}

void AArenaGameMode::PlayCombatSound()
{
	if (ChasedEnemies >= 2 && IsCombatSoundPlaying == false)
	{

		CombatAudioComponent = UGameplayStatics::SpawnSoundAttached(CombatSound, GetRootComponent());
		IsCombatSoundPlaying = true;
		CombatAudioComponent->Play(120);
		CombatAudioComponent->SetVolumeMultiplier(0.1);

	}


}

void AArenaGameMode::StopCombatSound()
{
	if (IsCombatSoundPlaying == true && !CombatAudioComponent) return;

	if (ChasedEnemies <= 0)
	{
		CombatAudioComponent->FadeOut(0.8f,0);
		IsCombatSoundPlaying = false;
		UE_LOG(LogTemp, Warning, TEXT("audio stopped"));
	}
}

void AArenaGameMode::RemoveEnemyFromWorld()
{
	TArray<AActor*> AllEnemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), AllEnemies);

	for (AActor* Actor : AllEnemies)
	{
		AEnemy* Enemy = Cast<AEnemy>(Actor);
		if (Enemy && KilledEnemiesNames.Contains(Enemy->EnemyName))
		{
			Enemy->Destroy();

			
		}
	}
}

void AArenaGameMode::RemoveItemFormWorld()
{
	TArray<AActor*> AllItems;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseItem::StaticClass(), AllItems);
	for (AActor* Actor : AllItems)
	{
		ABaseItem* Items = Cast<ABaseItem>(Actor);
		if (Items && AddedItems.Contains(Items->ItemID))
		{
			Items->Destroy();
		}
	}
}

void AArenaGameMode::IncrementEnemyAlive()
{

	EnemyAlive++;

}

void AArenaGameMode::DecrementEnemyAlive()
{
	EnemyAlive--;
	if (EnemySpawner->WaveMode == true)
	{
		CheckEnemy();

	}
}

void AArenaGameMode::CheckEnemy()
{

	if (EnemySpawner->WaveMode == true)
	{
		if (EnemyAlive < 1 && EnemySpawner)
		{
			NextWaveEnemyCount += 2;
			if (Implements<URespawnEnemyInterface>())
			{
				IRespawnEnemyInterface::Execute_RespawnEnemyStart(this);
			}
		}
	}
	
}
