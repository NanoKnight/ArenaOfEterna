#include "Items/EnemySpawner.h"
#include "Enemy/Enemy.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextRenderComponent.h"
#include"../WarriorCharacter.h"
#include "Components\WidgetComponent.h"
#include"Components\CapsuleComponent.h"
#include"Components\BoxComponent.h"
#include"Interfaces/RespawnEnemyInterface.h"
#include "Engine/World.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
    // Set this actor to call Tick() every frame.
    PrimaryActorTick.bCanEverTick = true;
    RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
    RootComponent = RootSceneComponent;
    SpawnerLocation = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpawnerLocation"));
    SpawnerLocation->SetupAttachment(RootComponent);
    SpawnerImage = CreateDefaultSubobject<UWidgetComponent>(TEXT("SpawnerImage"));
    SpawnerImage->SetupAttachment(RootComponent);
    TriggerSpawner = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBoxComponent"));
    TriggerSpawner->SetupAttachment(RootComponent);
    BlockBox = CreateDefaultSubobject<UBoxComponent>(TEXT("BlockBox"));
    BlockBox->SetupAttachment(RootComponent);
    SpawnerIDText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("SpawnerID"));
    SpawnerIDText->SetupAttachment(RootSceneComponent);

  
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
void AEnemySpawner::TriggerSpawnerCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

    if (bSpawned)return; 

    AWarriorCharacter* Warrior = Cast<AWarriorCharacter>(OtherActor);
    if (Warrior)
    {
        if (bSpawned == false)
        {
            bSpawned = true;
            SpawnEnemy(EnemySpawnCount);
            UE_LOG(LogTemp, Warning, TEXT("OtherComp: %s"), *GetNameSafe(OtherComp));

        }
    
    }
}
void AEnemySpawner::TriggerSpawnerCollisionEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{


}
// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{

    Super::BeginPlay();
    TriggerSpawner->OnComponentBeginOverlap.AddDynamic(this, &AEnemySpawner::TriggerSpawnerCollisionBeginOverlap);
    TriggerSpawner->OnComponentEndOverlap.AddDynamic(this, &AEnemySpawner::TriggerSpawnerCollisionEndOverlap);
    GetComponents<UBoxComponent>(CollisionBoxes);
    for (UBoxComponent* Box : CollisionBoxes)
    {
       if(BlockBox && Box->GetName().Contains(TEXT("BlockBox")))
       {

           Box->SetCollisionResponseToAllChannels(ECR_Ignore);
           BlockBoxes.Add(Box);

       }
    }

     GetComponents<UStaticMeshComponent>(SpawnerLocations);
        for (UStaticMeshComponent* SpawnerLoc : SpawnerLocations)
        {
            if (SpawnerLocation && SpawnerLoc->GetName().Contains(TEXT("SpawnerLoc")))
            {
                SpawnerLocations.Add(SpawnerLoc);
            }
        }


    SpawnDelegate.BindUObject(this, &AEnemySpawner::SpawnEnemy, EnemySpawnCount);
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
     

        float offset = 50.f;
        float Radius = 50.f;
        float AngelStep = 5.f / NumbwerOfEnemies;

        for (int32 i = 0; i < NumbwerOfEnemies; i++)
        {
            //float Angle = i * AngelStep;
            //float x = SpawnLocation.X + Radius * FMath::Cos(FMath::DegreesToRadians(Angle));
            //float y = SpawnLocation.Y + Radius * FMath::Sin(FMath::DegreesToRadians(Angle));

           // FVector NewspawnLocation(x, y, SpawnLocation.Z);
            //FVector NearestSpawnerLoc = GetActorLocation();
            //SpawnLocation.X += 100.f;
            //SpawnLocation.Y += 50.f;
            int32 MaxSpawnPointCount = SpawnerLocations.Num() - 1;
            int32 SelectedSpawnPoint = FMath::RandRange(0, MaxSpawnPointCount);
            FVector SpawnLocation = SpawnerLocations[SelectedSpawnPoint]->GetComponentLocation();
            AEnemy* SpawnedEnemy = GetWorld()->SpawnActor<AEnemy>(EnemyClass, SpawnLocation, FRotator::ZeroRotator);
            EnemyAlive++;
            

        }
        if (WaveCount > 0)
        {
            WaveCount--;

        }
    }
   

    if (!SpawnEnemiesLoc.IsZero())
    {
        int32 MaxSpawnPointCount = SpawnerLocations.Num() - 1;
        int32 SelectedSpawnPoint = FMath::RandRange(0, MaxSpawnPointCount);
        FVector SpawnLocation = SpawnerLocations[SelectedSpawnPoint]->GetComponentLocation();

        float offset = 300.f;
        float Radius = 400.f;
        float AngelStep = 260.f / NumbwerOfEnemies;

        for (int32 i = 0; i < NumbwerOfEnemies; i++)
        {
            //float Angle = i * AngelStep;
            //float x = SpawnLocation.X + Radius * FMath::Cos(FMath::DegreesToRadians(Angle));
            //float y = SpawnLocation.Y + Radius * FMath::Sin(FMath::DegreesToRadians(Angle));

            //FVector NewspawnLocation(x, y, SpawnLocation.Z);
            //FVector NearestSpawnerLoc = GetActorLocation();
            AEnemy* SpawnedEnemy = GetWorld()->SpawnActor<AEnemy>(EnemyClass, SpawnLocation, FRotator::ZeroRotator);
            EnemyAlive++;

        }
    }
  
   


    for (UBoxComponent* Box : CollisionBoxes)
    {
        if (BlockBox && Box->GetName().Contains(TEXT("BlockBox")))
        {

            Box->SetCollisionResponseToAllChannels(ECR_Block);

        }
    }
}

void AEnemySpawner::OnEnemyKilled()
{
    if (bSpawned)
    {
        EnemyAlive--;

        if (EnemyAlive <= 0 && WaveCount > 0)
        {
            GetWorld()->GetTimerManager().SetTimer(SpawnTimer, SpawnDelegate, SpawnTime, false);
        }

        if (EnemyAlive <= 0)
        {
            for (UBoxComponent* Box : CollisionBoxes)
            {
                if (BlockBox && Box->GetName().Contains(TEXT("BlockBox")))
                {

                    Box->SetCollisionResponseToAllChannels(ECR_Ignore);
                }

            }

        }
    }
 

}




