// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon.h"
#include "../WarriorCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components\SphereComponent.h"
#include "Components\BoxComponent.h"
#include"Components\InventorySystem\InventoryComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include"Interfaces\HitInterface.h"
#include"GameMode\ArenaGameMode.h"
#include"NiagaraComponent.h"



/******************GEREKSIZ KODLARI SIL DIKKATLI BAK********************************/

AWeapon::AWeapon()
{
	WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	WeaponBox->SetupAttachment(GetRootComponent());
	BoxTraceStart = CreateDefaultSubobject <USceneComponent>(TEXT("Start"));
	BoxTraceStart->SetupAttachment(GetRootComponent());
	BoxTraceEnd = CreateDefaultSubobject <USceneComponent>(TEXT("End"));
	BoxTraceEnd->SetupAttachment(GetRootComponent());
	WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);



}



void AWeapon::Equip(USceneComponent* InParent, FName InSocketName,AActor* NewOwner, APawn* NewInstigator)
{ 
	Super::Equip(InParent, InSocketName, NewOwner, NewInstigator);

}

void AWeapon::DeactivateEmbersEffect()
{
	Super::DeactivateEmbersEffect();

}

void AWeapon::DisableSphereCollision()
{
	Super::DisableSphereCollision();


}

void AWeapon::PlayEquipSound()
{
	Super::PlayEquipSound();
}

void AWeapon::PickUp(AWarriorCharacter* WarriorCharacter)
{
	Super::PickUp(WarriorCharacter);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap);
}

void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AWeapon::AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName)
{
	Super::AttachMeshToSocket(InParent, InSocketName);

}


void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (ActorIsSampleType(OtherActor))
	{
		return;

	}
	
	FHitResult BoxHit;
	BoxTrace(BoxHit);
	
	if (BoxHit.GetActor()) 
	{
		if (ActorIsSampleType(BoxHit.GetActor()))
		{
			return;
		}
		UGameplayStatics::ApplyDamage(BoxHit.GetActor(),WeaponDamage,GetInstigator()->GetController(),this,UDamageType::StaticClass());
		UE_LOG(LogTemp, Warning, TEXT("Applied %f damage to %s"), WeaponDamage, *BoxHit.GetActor()->GetName());
		ExecuteGetHit(BoxHit);

	}
}

bool AWeapon::ActorIsSampleType(AActor* OtherActor)
{
	return GetOwner()->ActorHasTag(TEXT("Enemy")) && OtherActor->ActorHasTag(TEXT("Enemy"));
}

void AWeapon::ExecuteGetHit(FHitResult& BoxHit)
{
	IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());

	if (HitInterface)
	{

		HitInterface->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint, GetOwner());

	}
	IgnoreActors.AddUnique(BoxHit.GetActor());	
	CreateFields(BoxHit.ImpactPoint);
}



void AWeapon::BoxTrace(FHitResult& BoxHit)
{
	const FVector Start = BoxTraceStart->GetComponentLocation();
	const FVector End = BoxTraceEnd->GetComponentLocation();

	TArray<AActor*> ActorsToIngnore;
	
	for (AActor* Actor : IgnoreActors)
	{
		
		ActorsToIngnore.AddUnique(Actor);
	}
	ActorsToIngnore.Add(GetOwner());


	ActorsToIngnore.Add(this);
	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,
		End,
		FVector(BoxTraceExtent),
		BoxTraceStart->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIngnore,
		bShowBoxDebug ? EDrawDebugTrace::ForDuration: EDrawDebugTrace::None,
		BoxHit,
		true

	);
	
	IgnoreActors.AddUnique(BoxHit.GetActor());	
    }



