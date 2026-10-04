// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include"../EnumStates.h"
#include "CombatStateInterFace.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCombatStateInterFace : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTX_API ICombatStateInterFace
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
   void SetEnemyState(EEnemyState NewState);
};
