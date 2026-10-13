// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"


class UCharacterHUD;
class UInventoryWidget;
class UInventoryComponent;
UCLASS()
class PROJECTX_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;


private:
		UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UCharacterHUD> CharacterHUDClass;
		UCharacterHUD* CharacterHUD;


		

public:

	

	FORCEINLINE UCharacterHUD* GetPlayerOverlay() const { return CharacterHUD; }


	
};
