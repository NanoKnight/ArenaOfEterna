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

void APlayerHUD::OpenInventory()
{
	UWorld* World = GetWorld();
	if (World)
	{
		if (!InventoryWidget) 
		{

		APlayerController* Controller = World->GetFirstPlayerController();
		if (Controller && InventoryWidgetClass)
		{
			
				InventoryWidget = CreateWidget<UInventoryWidget>(Controller, InventoryWidgetClass);
				InventoryWidget->AddToViewport();
				GEngine->AddOnScreenDebugMessage(0, 2.f, FColor::Cyan, FString::Printf(TEXT("First Opened")));
     			Controller->bShowMouseCursor = true;

		}	
	   }
		
		if (InventoryWidget)
		{
			if (InventoryWidget->GetVisibility() == ESlateVisibility::Visible)
			{
				InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
			}
			else
			{
				InventoryWidget->SetVisibility(ESlateVisibility::Visible);

			}

		}

	}
}

