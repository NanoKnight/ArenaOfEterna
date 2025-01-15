// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnManager.generated.h"

class AEnemySpawner;
UCLASS()
class PROJECTX_API ASpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Spawner")
	void TriggerSpawnerByID(int32 SpawnerID);

	AEnemySpawner* GetNextSpawn();


private:
	TArray<AEnemySpawner*> Spawners;
	int32 CurrentSpawnerIndex;

};
