// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/BaseItem.h"
#include "Kismet/GameplayStatics.h"
#include "Components\SphereComponent.h"
#include"NiagaraComponent.h"
#include"../DebugMacros.h"
#include"../WarriorCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include"Interfaces\HitInterface.h"
#include"Components\WidgetComponent.h"
#include"HUD\ItemInteractionWidget.h"
#include"Components\InventorySystem\InventoryComponent.h"
#include"GameMode\ArenaGameMode.h"



// Sets default values
ABaseItem::ABaseItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = ItemMesh;
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(GetRootComponent());
	ItemEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara"));
	ItemEffect->SetupAttachment(GetRootComponent());
	ItemInteraction = CreateDefaultSubobject<UWidgetComponent>(TEXT("ItemInteraction"));
	ItemInteraction->SetupAttachment(GetRootComponent());

}

void ABaseItem::Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator)
{
	
	ItemState = EItemState::EIS_Equipped;
	SetOwner(NewOwner);
	SetInstigator(NewInstigator);
	AttachMeshToSocket(InParent, InSocketName);
	DisableSphereCollision();
	PlayEquipSound();
	DeactivateEmbersEffect();
}

void ABaseItem::AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName)
{
	if (ItemMesh)
	{
		

		FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
		ItemMesh->AttachToComponent(InParent, TransformRules, InSocketName);
		ItemMesh->SetRelativeLocation(FVector::ZeroVector);
		ItemMesh->SetRelativeRotation(FRotator::ZeroRotator);
	
	}

}

void ABaseItem::DeactivateEmbersEffect()
{
	if (ItemEffect)
	{
		ItemEffect->Deactivate();
	}
}

void ABaseItem::DisableSphereCollision()
{
	if (Sphere)
	{
		Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ABaseItem::PlayEquipSound()
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

void ABaseItem::SetInteractionVisibility(bool visiblity)
{
	if (ItemState != EItemState::EIS_Equipped)
	{
		ItemInteraction->SetVisibility(visiblity, false);

	}


}

void ABaseItem::PickUp(AWarriorCharacter* WarriorCharacter)
{	
	
	for (const FInventoryStruct& Item: WarriorCharacter->GetInventoryComponent()->InventoryItems)
	{
		if (Item.ItemName.IsEmpty()) 
		{
			if (WarriorCharacter && WarriorCharacter->GetInventoryComponent())
			{
				AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
				AArenaGameMode* ArenaGameMode = Cast<AArenaGameMode>(GameMode);
				if (ArenaGameMode)
				{
					ArenaGameMode->AddedItems.Add(ItemID);
				}

				FInventoryStruct NewItem;
				NewItem.ItemName = ItemName;				
				NewItem.ItemIcon = ItemIcon;
				NewItem.EquipmentSlot = EEquipmentSlot::Weapon;
				NewItem.ItemClass = this->GetClass();
				NewItem.ItemStaticMesh = ItemMesh->GetStaticMesh();
				NewItem.ItemTypes = ItemType;
				NewItem.ItemSocketName = ItemSocketName;
				NewItem.EquipmentSlot = ItemEquipmentSlot;
				NewItem.Defense = Defense;
				NewItem.Damage = Damage;
				NewItem.HealthValue = HealthValue;
				
				WarriorCharacter->GetInventoryComponent()->AddItem(NewItem);
				this->Destroy();
				break;


			
			}
		}
		else 
		{

			WarriorCharacter->GetInventoryComponent()->InventoryFullText();
		}
	}



}

// Called when the game starts or when spawned
void ABaseItem::BeginPlay()
{
	Super::BeginPlay();
	ItemID = GetName();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ABaseItem::OnSphereOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &ABaseItem::OnSphereEndOverlap);
	
}

void ABaseItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AWarriorCharacter* WarriorCharacter = Cast<AWarriorCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	IPickUpInterface* ItemActorInterface = Cast<IPickUpInterface>(OtherActor);
	if (ItemActorInterface)
	{
		ItemActorInterface->SetOverlappingItem(this);
	}

	ItemInteraction->SetVisibility(true);

	UUserWidget* UserWidget = ItemInteraction->GetUserWidgetObject();
	if (UserWidget)
	{
		UItemInteractionWidget* ItemInteractWidget = Cast<UItemInteractionWidget>(UserWidget);
		if (ItemInteractWidget)
		{
			if (ItemType == EItemTypes::Weapon)
			{
				ItemInteractWidget->SetInteractionValues(ItemName, Damage, ItemType);

			}
			else
			{
				ItemInteractWidget->SetInteractionValues(ItemName, Defense, ItemType);

			}

		}
	}
}

void ABaseItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IPickUpInterface* ItemActorInterface = Cast<IPickUpInterface>(OtherActor);
	if (ItemActorInterface)
	{
		ItemActorInterface->SetOverlappingItem(nullptr);

	}
	ItemInteraction->SetVisibility(false);




}

// Called every frame
void ABaseItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

