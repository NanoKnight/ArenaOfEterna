// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/ArenaGameMode.h"
#include"Enemy\Enemy.h"
#include"Components\AttributeComponent.h"
#include "SaveGames/EternaSaveGame.h"
#include"Items\ItemActor.h"
#include "Kismet/GameplayStatics.h"
#include "./Items/EnemySpawner.h"
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
   /*	EnemySpawner = Cast<AEnemySpawner>(UGameplayStatics::GetActorOfClass(GetWorld(), AEnemySpawner::StaticClass()));
	NextWaveEnemyCount = EnemySpawner->EnemySpawnCount;

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
		SaveGameObject->EquippedWeapon  = WarriorCharacter->EquippedWeapon->GetClass();
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
			WaveCount = SaveGameObject->WaveCount;
			NextWaveEnemyCount = SaveGameObject->EnemyNextWaveCount;
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
			WarriorCharacter->WeaponClass = SaveGameObject->EquippedWeapon;

		}
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
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AItemActor::StaticClass(), AllItems);
	for (AActor* Actor : AllItems)
	{
		AItemActor* Items = Cast<AItemActor>(Actor);
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
