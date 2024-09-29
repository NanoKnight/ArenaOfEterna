// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PickUpInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPickUpInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTX_API IPickUpInterface
{
	GENERATED_BODY()

public:
	virtual void SetOverlappingItem(class AItemActor* Item);
	virtual void  AddXp(class AExperiencePoint* Xp);
	virtual void AddGold(class ATreasure* Treasure);
	virtual void AddHealth(class AHealthPoint* Health);
};
