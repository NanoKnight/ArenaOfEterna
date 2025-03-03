// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Collectable.h"
#include "ExperiencePoint.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTX_API AExperiencePoint : public ACollectable
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay();
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
	UPROPERTY(EditAnywhere)
	int32 ExperiencePoints;

	

public:
	AExperiencePoint();
	FORCEINLINE int32 GetExperience() const { return ExperiencePoints; }
	FORCEINLINE void SetExp(int32 NumberOfExp) { ExperiencePoints = NumberOfExp; }
};
