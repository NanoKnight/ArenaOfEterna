// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/ItemActor.h"
#include "Treasure.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTX_API ATreasure : public AItemActor
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay()override;
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;

private:
	UPROPERTY(EditAnywhere, Category = "WeaponProperties")
	USoundBase* PickUp;

	UPROPERTY(EditAnywhere, Category = "Treasure properties")
	int32 Gold;
public:
	FORCEINLINE int32 GetGold() const { return Gold; }
	
};
