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

	TArray<UStaticMeshComponent*> AllMeshes;
     GetComponents<UStaticMeshComponent>(AllMeshes);
     SpawnerLocations.Empty();
        for (UStaticMeshComponent* SpawnerLoc : AllMeshes)
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


    if (!EnemyClass || SpawnerLocations.Num() == 0)
    {
        return;
    }
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride =
        ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;


    for (int32 i = 0; i < NumbwerOfEnemies; i++)
    {
        
        const int32 SpawnPointIndex =
            i % SpawnerLocations.Num();

        UStaticMeshComponent* SpawnPoint =
            SpawnerLocations[SpawnPointIndex];

        if (!IsValid(SpawnPoint))
        {
            continue;
        }

        const FVector SpawnLocation =
            SpawnPoint->GetComponentLocation();

        AEnemy* SpawnedEnemy =
            GetWorld()->SpawnActor<AEnemy>(
                EnemyClass,
                SpawnLocation,
                FRotator::ZeroRotator,
                SpawnParams
            );

        if (SpawnedEnemy)
        {
            EnemyAlive++;
        }
    }

    if (WaveCount > 0)
    {
        WaveCount--;
    }

    for (UBoxComponent* Box : CollisionBoxes)
    {
        if (IsValid(Box) &&
            Box->GetName().Contains(TEXT("BlockBox")))
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




