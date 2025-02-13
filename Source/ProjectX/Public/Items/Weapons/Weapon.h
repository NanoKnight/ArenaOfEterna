// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/ItemActor.h"
#include"./InterFaces/HitInterface.h"
#include "../../Structs/InventoryStruct.h"
#include "Weapon.generated.h"

class USoundBase;
class UBoxComponent;
class AWarriorCharacter;

/**
 * 
 */
UCLASS()
class PROJECTX_API AWeapon : public AItemActor , public IHitInterface
{
	GENERATED_BODY()
public:

	AWeapon();
	void Equip(USceneComponent* InParent, FName InSocketName,AActor * NewOwner , APawn* NewInstigator );

	void DeactivateEmbersEffect();

	void DisableSphereCollision();

	void PlayEquipSound();

	void PickUp(AWarriorCharacter* WarriorCharacter);

	void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName);

	//****///

	TArray <AActor*> IgnoreActors;

	UWorld* World = GetWorld();
	AActor* TraceActor = GetOwner();

	
	
	
protected:
	virtual void BeginPlay() override;
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	bool ActorIsSampleType(AActor* OtherActor);

	void ExecuteGetHit(FHitResult& BoxHit);

	//For Edit From BP
	UFUNCTION(BlueprintImplementableEvent)
	void CreateFields(const FVector& FieldLocation);


private:
	void BoxTrace(FHitResult& BoxHit);

	UPROPERTY(EditAnywhere)
	FString ItemName;
	UPROPERTY(EditAnywhere)
	UTexture2D* ItemIcon;

	UPROPERTY(VisibleAnywhere, Category = "WeaponProperties")
	UBoxComponent* WeaponBox;

	UPROPERTY(EditAnywhere, Category = "WeaponProperties")
	FVector BoxTraceExtent = FVector(10.f);

	UPROPERTY(EditAnywhere, Category = "WeaponProperties")
	bool bShowBoxDebug = false;

	UPROPERTY(EditAnywhere, Category = "WeaponProperties")
	USoundBase* EquipSound;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceStart;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceEnd;

	UPROPERTY(EditAnywhere, Category = "WeaponProperties")
	float Damage = 20.f;



	

public:
	FORCEINLINE UBoxComponent* GetWeaponBox()  const { return WeaponBox; }
	FORCEINLINE float GetDamage() const { return Damage;}
	
	
	
	FORCEINLINE void SetDamage(float NewDamage) { Damage = NewDamage; }


};
