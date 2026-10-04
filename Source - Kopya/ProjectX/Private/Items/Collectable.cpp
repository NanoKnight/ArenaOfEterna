// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Collectable.h"
#include "Components/SphereComponent.h"
#include"Interfaces\PickUpInterface.h"
#include"../DebugMacros.h"
#include "../WarriorCharacter.h"
#include"GameMode\ArenaGameMode.h"
#include "Kismet/GameplayStatics.h"
#include"NiagaraComponent.h"




// Sets default values
ACollectable::ACollectable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true; 

}

// Called when the game starts or when spawned
void ACollectable::BeginPlay()
{
	Super::BeginPlay();



}

// Called every frame
void ACollectable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



}

void ACollectable::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AWarriorCharacter* WarriorCharacter = Cast<AWarriorCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	IPickUpInterface* CollectableActorInterface = Cast<IPickUpInterface>(OtherActor);
	if (CollectableActorInterface)
	{
		CollectableActorInterface->SetOverlappingItem(this);
		AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
		AArenaGameMode* ArenaGameMode = Cast<AArenaGameMode>(GameMode);
		if (ArenaGameMode)
		{
			ArenaGameMode->AddedItems.Add(ItemID);
		}						
	}	
}

void ACollectable::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
   IPickUpInterface* ItemActorInterface = Cast<IPickUpInterface>(OtherActor);
	if (ItemActorInterface)
	{
		ItemActorInterface->SetOverlappingItem(nullptr);
	}

}





