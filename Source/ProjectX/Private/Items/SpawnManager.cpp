// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/SpawnManager.h"
#include "./Items/EnemySpawner.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASpawnManager::ASpawnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void ASpawnManager::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*>FoundSpawners;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemySpawner::StaticClass(), FoundSpawners);
	for (AActor* SpawnersActor : FoundSpawners)
	{
		AEnemySpawner* Spawner = Cast<AEnemySpawner>(SpawnersActor);
		if (Spawner)
		{
			if (Spawner->Loop) {
				LoopedSpawners.Add(Spawner);

		  }
			else
			{
				Spawners.Add(Spawner);
			}
		}		
	}

	CurrentSpawnerIndex = 1;
	Spawners.Sort([](const AEnemySpawner& A, const AEnemySpawner& B) {
		return A.SpawnerID < B.SpawnerID;
		});

	if (Spawners.Num() > 0 )
	{
		
		TriggerSpawnerByID(Spawners[0]->SpawnerID);
	}
	
	for (AEnemySpawner* Spawner:LoopedSpawners)
	{
		if (Spawner) {
			Spawner->SpawnEnemy(Spawner->EnemySpawnCount);
		}
	}

}

// Called every frame
void ASpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawnManager::TriggerSpawnerByID(int32 SpawnerID)
{
	for (AEnemySpawner* Spawner : Spawners)
	{
		if(Spawner->SpawnerID == SpawnerID) 
		{
			Spawner->SpawnEnemy(Spawner->EnemySpawnCount);
			break;
		}
	}

}

AEnemySpawner* ASpawnManager::GetNextSpawn()
{

	if (Spawners.IsValidIndex(CurrentSpawnerIndex))
	{
		AEnemySpawner* NextSpawner = Spawners[CurrentSpawnerIndex];
		CurrentSpawnerIndex++;
		return NextSpawner;
	}
	return nullptr;
}



