// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Weapon.h"
#include "../WarriorCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components\SphereComponent.h"
#include "Components\BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include"Interfaces\HitInterface.h"
#include"GameMode\ArenaGameMode.h"
#include"NiagaraComponent.h"


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
	ItemState = EItemState::EIS_Equipped;
	SetOwner(NewOwner);
	SetInstigator(NewInstigator);
	AttachMeshToSocket(InParent, InSocketName);
	DisableSphereCollision();
	PlayEquipSound();
	DeactivateEmbersEffect();

	AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
	AArenaGameMode* ArenaGameMode = Cast<AArenaGameMode>(GameMode);
	if (ArenaGameMode)
	{
		ArenaGameMode->AddedItems.Add(ItemName);
	}
}

void AWeapon::DeactivateEmbersEffect()
{
	if (ItemEffect)
	{
		ItemEffect->Deactivate();
	}
}

void AWeapon::DisableSphereCollision()
{
	if (Sphere)
	{
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

}

void AWeapon::PlayEquipSound()
{
	if (EquipSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			EquipSound,
			GetActorLocation()
		);
	}
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap);
}

void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AWarriorCharacter* WarriorCharacter = Cast<AWarriorCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	IPickUpInterface* ItemActorInterface = Cast<IPickUpInterface>(OtherActor);
	if (ItemActorInterface)
	{
		ItemActorInterface->SetOverlappingItem(this);
	}
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	IPickUpInterface* ItemActorInterface = Cast<IPickUpInterface>(OtherActor);
	if (ItemActorInterface)
	{
		ItemActorInterface->SetOverlappingItem(nullptr);

	}
}

void AWeapon::AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(InParent, TransformRules, InSocketName);
}
/**********************************/

void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (ActorIsSampleType(OtherActor))
	{
		return;
	}
	
	FHitResult BoxHit;
	BoxTrace(BoxHit);
	
	if (BoxHit.GetActor()) {
		if (ActorIsSampleType(BoxHit.GetActor()))
		{
			return;
		}
		UGameplayStatics::ApplyDamage(	BoxHit.GetActor(),Damage,GetInstigator()->GetController(),this,UDamageType::StaticClass());

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



