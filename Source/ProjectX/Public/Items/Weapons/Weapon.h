// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Collectable.h"
#include"../BaseItem.h"
#include"./InterFaces/HitInterface.h"
#include "../../Structs/InventoryStruct.h"
#include "Weapon.generated.h"



/**
 * 
 */
UCLASS()
class PROJECTX_API AWeapon : public ABaseItem, public IHitInterface
{
	GENERATED_BODY()
public:

	AWeapon();

	virtual void Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator);

	virtual void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName);

	virtual void DeactivateEmbersEffect();

	virtual void DisableSphereCollision();

	virtual void PlayEquipSound();

	void PickUp(AWarriorCharacter* WarriorCharacter);


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

	UPROPERTY(VisibleAnywhere, Category = "WeaponProperties")
	UBoxComponent* WeaponBox;

	UPROPERTY(EditAnywhere, Category = "WeaponProperties")
	FVector BoxTraceExtent = FVector(10.f);

	UPROPERTY(EditAnywhere, Category = "WeaponProperties")
	bool bShowBoxDebug = false;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceStart;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceEnd;

	UPROPERTY(EditAnywhere, Category = "WeaponProperties")
	float WeaponDamage = 20.f;



	

public:

	FORCEINLINE UBoxComponent* GetWeaponBox()  const { return WeaponBox; }
	FORCEINLINE float GetDamage() const { return WeaponDamage;}
	FORCEINLINE void SetDamage(float NewDamage) { WeaponDamage = NewDamage; }


};
