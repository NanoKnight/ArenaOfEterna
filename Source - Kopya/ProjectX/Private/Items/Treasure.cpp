// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Treasure.h"
#include"Interfaces\PickUpInterface.h"
#include"Kismet/GameplayStatics.h"

void ATreasure::BeginPlay()
{
	Super::BeginPlay();

}

void ATreasure::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent,OtherActor,OtherComp,OtherBodyIndex,bFromSweep,SweepResult);

	IPickUpInterface* ItemActorInterface = Cast<IPickUpInterface>(OtherActor);
	if (ItemActorInterface)
	{
		ItemActorInterface->AddGold(this);

		if (PickUp)
		{
			UGameplayStatics::PlaySoundAtLocation(
				this,
				PickUp,
				GetActorLocation()
			);
		}
		Destroy();


	}


}

