// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerHUD.h"
#include "HUD\CharacterHUD.h"
#include"Components\InventorySystem\InventoryComponent.h"
#include "HUD\InventoryWidget.h"

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* Controller = World->GetFirstPlayerController();

		if (Controller && CharacterHUDClass)
		{
			CharacterHUD = CreateWidget<UCharacterHUD>(Controller, CharacterHUDClass);
			CharacterHUD->AddToViewport();
		}
	}

	
}



