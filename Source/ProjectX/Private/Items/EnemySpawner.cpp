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

void AEnemySpawner::RespawnEnemyStart_Implementation()
{
 
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AEnemySpawner::SpawnEnemy(int32 NumbwerOfEnemies)
{

    if (SpawnEnemiesLoc.IsZero())
    {
        FVector SpawnLocation = GetActorLocation();

        AEnemy* SpawnedEnemy = GetWorld()->SpawnActor<AEnemy>(EnemyClass, SpawnLocation, FRotator::ZeroRotator);
        if (SpawnedEnemy)
        {

            SetLifeSpan(1.0f);
        }
    }
    if (!SpawnEnemiesLoc.IsZero())
    {

        FVector SpawnLocation = SpawnEnemiesLoc;

        float offset = 300.f;
        float Radius = 400.f;
        float AngelStep = 260.f / NumbwerOfEnemies;

        for (int32 i = 0; i < NumbwerOfEnemies; i++)
        {
            float Angle = i * AngelStep;
            float x = SpawnLocation.X + Radius * FMath::Cos(FMath::DegreesToRadians(Angle));
            float y = SpawnLocation.Y + Radius * FMath::Sin(FMath::DegreesToRadians(Angle));

            FVector NewspawnLocation(x, y, SpawnLocation.Z);
            FVector NearestSpawnerLoc = GetActorLocation();
            SpawnLocation.X += 100.f;
            SpawnLocation.Y += 50.f;

            AEnemy* SpawnedEnemy = GetWorld()->SpawnActor<AEnemy>(EnemyClass, NewspawnLocation, FRotator::ZeroRotator);
            if (SpawnedEnemy)
            {

                SetLifeSpan(1.0f);
            }
        }
    }
  
   
}




