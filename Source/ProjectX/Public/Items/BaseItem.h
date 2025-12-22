// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include"./InterFaces/HitInterface.h"
#include "../Structs/InventoryStruct.h"
#include "BaseItem.generated.h"


class USphereComponent;
class USoundBase;
class UBoxComponent;
class UWidgetComponent;
class AWarriorCharacter;



enum class EItemState : uint8
{
	EIS_Hovering,
	EIS_Equipped
};

UCLASS()
class PROJECTX_API ABaseItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseItem();


	void Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator);

	void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName);

	void DeactivateEmbersEffect();

	void DisableSphereCollision();

	void PlayEquipSound();

	void PickUp(AWarriorCharacter* WarriorCharacter);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	

	/* Itemi sahnede deðiþtirdiðinde kuþandýlan iteme etki etmiyor bunun için kuþanýrken yerden adlýðým itemlerin deðerlerini gircen*/


	UPROPERTY(VisibleAnywhere, Category = "WeaponProperties")
	USoundBase* EquipSound;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UStaticMeshComponent* ItemMesh;

	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* ItemInteraction;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* Sphere;

	UPROPERTY(VisibleAnywhere)
	class UNiagaraComponent* ItemEffect;




	UPROPERTY(VisibleAnywhere)
	TSubclassOf<UUserWidget> WidgetClass;


	EItemState ItemState = EItemState::EIS_Hovering;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



	UPROPERTY()
	FString ItemID;

	UPROPERTY(EditAnywhere)
	EItemTypes ItemType;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "ItemType != EItemTypes::Pot", EditConditionHides))
	EEquipmentSlot ItemEquipmentSlot;

	UPROPERTY(EditAnywhere,meta = (EditCondition = "ItemType == EItemTypes::Weapon", EditConditionHides))
	float Damage = 20.f;
	
	UPROPERTY(EditAnywhere, meta = (EditCondition = "ItemType != EItemTypes::Weapon && ItemTypes != EItemTypes::Pot", EditConditionHides))
	float Defense;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "ItemType == EItemTypes::Pot", EditConditionHides))
	float HealthValue;


	UPROPERTY(EditAnywhere)
	FString ItemName;

	UPROPERTY(EditAnywhere)
	UTexture2D* ItemIcon;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "ItemType != EItemTypes::Pot", EditConditionHides))
	FName ItemSocketName;

	void SetInteractionVisibility(bool visiblity);

	FORCEINLINE UStaticMeshComponent* GetItemMesh() const { return ItemMesh; }
	FORCEINLINE UWidgetComponent* GetItemInteractionWidget() const{ return ItemInteraction; }

};
