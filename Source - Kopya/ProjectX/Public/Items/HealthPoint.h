// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Collectable.h"
#include "HealthPoint.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTX_API AHealthPoint : public ACollectable
{
	GENERATED_BODY()

protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	UPROPERTY(EditAnywhere)
	float HealthPoint;

public:
	FORCEINLINE float GetHealth() const{ return HealthPoint; }
};
