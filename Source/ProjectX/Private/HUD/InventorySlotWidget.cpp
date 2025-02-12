// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/InventorySlotWidget.h"
#include "Components/Image.h"
#include"../WarriorCharacter.h"
#include"Components\Button.h"
#include "Components/TextBlock.h"

void UInventorySlotWidget::SetUp(const FInventoryStruct& NewItem)
{

	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString(TEXT("SetUp Called!")));
	Item = NewItem;
	if (ItemButton)
	{
		ItemButton->OnClicked.AddDynamic(this, &UInventorySlotWidget::OnItemClicked);
	}

	if (ItemIcon && Item.ItemIcon)
	{
		ItemIcon->SetBrushFromTexture(Item.ItemIcon);
	}

	if (ItemName)
	{
		ItemName->SetText(FText::FromString(Item.ItemName));
	}

	if (!ItemButton)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ItemButton is NULL!"));
	}
}

void UInventorySlotWidget::OnItemClicked()
{
	AWarriorCharacter* Warrior = Cast<AWarriorCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (Warrior)
	{
		Warrior->EquipItem(Item);
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("Equipped")));
	}

}
