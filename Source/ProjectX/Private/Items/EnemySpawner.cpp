#include "Items/EnemySpawner.h"
#include "Enemy/Enemy.h"
#include "Kismet/GameplayStatics.h"
#include"Interfaces/RespawnEnemyInterface.h"
#include "Engine/World.h"


// Sets default values
AEnemySpawner::AEnemySpawner()
{
    // Set this actor to call Tick() every frame.
    PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
    Super::BeginPlay();
 
}

void AEnemySpawner::RespawnEnemy()
{
  
    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString(TEXT("denemee1")));
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AEnemySpawner::SpawnEnemy(int32 NumbwerOfEnemies)
{
    FVector SpawnLocation = GetActorLocation();
    float offset = 200.f;

    for (int32 i = 0; i < NumbwerOfEnemies; i++)
    {
        GetWorld()->SpawnActor<AEnemy>(EnemyClass, SpawnLocation, FRotator::ZeroRotator);

        SpawnLocation.X += 200.f;
    }



    /*
    FVector SpawnLocation = GetActorLocation();
    FRotator SpawnRotation = GetActorRotation();
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    AEnemy* SpawnedEnemy = GetWorld()->SpawnActor<AEnemy>(EnemyClass, SpawnLocation, SpawnRotation, SpawnParams);
    */
}

void AEnemySpawner::GenerateSpawnerID()
{

}

