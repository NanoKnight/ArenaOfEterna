// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include"../Enemy/Enemy.h"
#include "EnemySpawner.generated.h"
class AEnemy;
class UCapsuleComponent;
class UBoxComponent;
class AWarriorCharacter;

UCLASS()

class PROJECTX_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

protected:
	// Called when the game starts or when spawned

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneComponent* RootSceneComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCapsuleComponent* CapsuleComponent;

    UPROPERTY(EditAnywhere)
	class UBoxComponent* TriggerSpawner;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* BlockBox;
	TArray<UBoxComponent*>CollisionBoxes;

	UPROPERTY(EditAnywhere)
	TArray<UBoxComponent*>BlockBoxes;


	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UTextRenderComponent* SpawnerIDText;

	virtual void OnConstruction(const FTransform& Transform) override;


	UFUNCTION()
	void TriggerSpawnerCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void TriggerSpawnerCollisionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	
	
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void SpawnEnemy(int32 NumberOfEnemies);
	
	
	UPROPERTY(EditAnywhere,Category ="Default")
	TSubclassOf<AEnemy>EnemyClass;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	bool WaveMode;

	UPROPERTY(EditAnywhere, Category = "Default")
	bool Loop;

	UPROPERTY(EditAnywhere, Category = "Default")
	int32 EnemySpawnCount;

	UPROPERTY(EditAnywhere, Category = "Default")
	int32 WaveCount;

	UPROPERTY(EditAnywhere, Category = "Default")
	int32 SpawnerID;

	UPROPERTY(EditAnywhere, Category = "Default")
	FVector SpawnEnemiesLoc;

	int32 EnemyAlive;

	FTimerHandle SpawnTimer;

	FTimerDelegate SpawnDelegate;

	UPROPERTY(EditAnywhere,Category = "Default")
	float SpawnTime = 0.5f;

	void OnEnemyKilled();

	
private:

	bool bSpawned = false;

};
