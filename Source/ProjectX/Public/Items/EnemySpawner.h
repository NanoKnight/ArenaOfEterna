// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include"../Enemy/Enemy.h"
#include"../Interfaces/RespawnEnemyInterface.h"
#include "EnemySpawner.generated.h"
class AEnemy;

UCLASS()

class PROJECTX_API AEnemySpawner : public AActor, public IRespawnEnemyInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

protected:
	// Called when the game starts or when spawned





public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	virtual void RespawnEnemyStart_Implementation()override;
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void SpawnEnemy(int32 NumberOfEnemies);
	
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AEnemy>EnemyClass;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool WaveMode;

	UPROPERTY(EditAnywhere)
	bool Disposable;

	UPROPERTY(EditAnywhere)
	int32 EnemySpawnCount;

	UPROPERTY(EditAnywhere)
	int32 SpawnerID;

	UPROPERTY(EditAnywhere)
	FVector SpawnEnemiesLoc;


	
private:


};
