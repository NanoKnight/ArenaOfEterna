// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveGames/EternaSaveGame.h"
#include"../WarriorCharacter.h"
#include "GameFramework/SaveGame.h"




void UEternaSaveGame::AddKilledEnemy(FString EnemyName)
{
	if (!KilledEnemiesName.Contains(EnemyName))
	{
		KilledEnemiesName.Add(EnemyName);
		
	}
}


void UEternaSaveGame::AddItems(FString AddedItemsName)
{
	if (!AddedItems.Contains(AddedItemsName))
	{
		AddedItems.Add(AddedItemsName);

	}
}

