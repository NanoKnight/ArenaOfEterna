// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/HealthPoint.h"
#include"Interfaces\PickUpInterface.h"

void AHealthPoint::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IPickUpInterface* ItemActorInterface = Cast<IPickUpInterface>(OtherActor);
	if (ItemActorInterface)
	{
		ItemActorInterface->AddHealth(this);
		Destroy();
	}
}
