// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include"../BaseItem.h"
#include"./InterFaces/HitInterface.h"
#include "Shield.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTX_API AShield : public  ABaseItem,  public IHitInterface
{
	GENERATED_BODY()
	
public:
	AShield();
	virtual void Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator);


protected:

private:



};
