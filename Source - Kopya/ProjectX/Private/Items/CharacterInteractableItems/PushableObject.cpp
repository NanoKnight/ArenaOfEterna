// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/CharacterInteractableItems/PushableObject.h"
#include"Components\SphereComponent.h"
#include"Interfaces\CharacterInteractableInterFace.h"






// Sets default values
APushableObject::APushableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetupAttachment(Mesh);
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(Mesh);



}

// Called when the game starts or when spawned
void APushableObject::BeginPlay()
{
	Super::BeginPlay();


	if (Sphere)
	{
		Sphere->OnComponentBeginOverlap.AddDynamic(this, &APushableObject::OnSphereOverlap);
		Sphere->OnComponentEndOverlap.AddDynamic(this, &APushableObject::OnSphereEndOverlap);

	}

	Mesh->BodyInstance.bLockXRotation = true;
	Mesh->BodyInstance.bLockYRotation = true;
	Mesh->BodyInstance.bLockZRotation = true;


	
}

void APushableObject::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor)
	{
		return;
	}

	if (OtherActor->Implements<UCharacterInteractableInterFace>())
	{
		ICharacterInteractableInterFace* Interface = Cast<ICharacterInteractableInterFace>(OtherActor);
		if (Interface)
		{
			Interface->CharacterInteract(this);
		}
	}

}

void APushableObject::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor)
	{
		return;
	}

	ICharacterInteractableInterFace* Interface = Cast<ICharacterInteractableInterFace>(OtherActor);
	if (Interface)
	{
		Interface->CharacterInteract(nullptr);

	}
	


}

// Called every frame
void APushableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



