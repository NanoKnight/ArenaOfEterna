// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ExperiencePoint.h"
#include"Interfaces\PickUpInterface.h"

#include "SaveGames/EternaSaveGame.h"


AExperiencePoint::AExperiencePoint()
{
}
void AExperiencePoint::BeginPlay()
{
	Super::BeginPlay();


}
void AExperiencePoint::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent,OtherActor,  OtherComp,OtherBodyIndex,bFromSweep,SweepResult);
	IPickUpInterface* ItemActorInterface = Cast<IPickUpInterface>(OtherActor);

	if (ItemActorInterface)
	{
		//GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Green, ItemName);
		ItemActorInterface->AddXp(this);
		Destroy();
	}
}

