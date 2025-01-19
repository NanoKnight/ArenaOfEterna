#include "Items/EnemySpawner.h"
#include "Enemy/Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextRenderComponent.h"
#include"Components\CapsuleComponent.h"
#include"Interfaces/RespawnEnemyInterface.h"
#include "Engine/World.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
    // Set this actor to call Tick() every frame.
    PrimaryActorTick.bCanEverTick = true;
    RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
    RootComponent = RootSceneComponent;
    SpawnerIDText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("SpawnerID"));
    SpawnerIDText->SetupAttachment(RootSceneComponent);

    CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
    CapsuleComponent->SetupAttachment(GetRootComponent());

}


void AEnemySpawner::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    if (SpawnerIDText && !Loop)
    {
        
        SpawnerIDText->SetText(FText::FromString(FString::Printf(TEXT("Spawner ID = %d"), SpawnerID)));
    }
    if (Loop)
    {
        SpawnerIDText->SetText(FText::FromString(FString::Printf(TEXT("Spawner Infinite"))));

    }

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

        float offset = 300.f;
        float Radius = 100.f;
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
            if (SpawnedEnemy && !Loop)
            {

                SetLifeSpan(1.0f);
            }
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
            if (SpawnedEnemy && !Loop)
            {

                SetLifeSpan(1.0f);
            }
        }
    }
  
   
}




