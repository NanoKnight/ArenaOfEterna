// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemActor.h"
#include "Components/SphereComponent.h"
#include"Interfaces\PickUpInterface.h"
#include"../DebugMacros.h"
#include "../WarriorCharacter.h"
#include"GameMode\ArenaGameMode.h"
#include "Kismet/GameplayStatics.h"
#include"NiagaraComponent.h"




// Sets default values
AItemActor::AItemActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true; 
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = ItemMesh;
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());
	ItemEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	ItemEffect->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void AItemActor::BeginPlay()
{
	Super::BeginPlay();
	ItemName = GetName();
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItemActor::OnSphereOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AItemActor::OnSphereEndOverlap);


}

// Called every frame
void AItemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



}

void AItemActor::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AWarriorCharacter* WarriorCharacter = Cast<AWarriorCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	IPickUpInterface* ItemActorInterface = Cast<IPickUpInterface>(OtherActor);
	if (ItemActorInterface)
	{


		ItemActorInterface->SetOverlappingItem(this);



		AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
		AArenaGameMode* ArenaGameMode = Cast<AArenaGameMode>(GameMode);
		if (ArenaGameMode)
		{
			ArenaGameMode->AddedItems.Add(ItemName);
		}


		
				
	}	
}

void AItemActor::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IPickUpInterface* ItemActorInterface = Cast<IPickUpInterface>(OtherActor);
	if (ItemActorInterface)
	{
		ItemActorInterface->SetOverlappingItem(nullptr);

	}

}

