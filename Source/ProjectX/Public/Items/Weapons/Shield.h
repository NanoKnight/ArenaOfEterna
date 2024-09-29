// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/ItemActor.h"
#include"./InterFaces/HitInterface.h"
#include "Shield.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTX_API AShield : public AItemActor , public IHitInterface
{
	GENERATED_BODY()
	
public:
	AShield();
	void Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator);


protected:

private:



};
