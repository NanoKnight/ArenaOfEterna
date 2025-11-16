// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/InventoryWidget.h"
#include "Components/GridPanel.h"
#include"Components\GridSlot.h"
#include"HUD\EqiupmentSlotWidget.h"
#include "Blueprint/DragDropOperation.h"
#include"./Components/TextBlock.h"
#include"Components\InventorySystem\InventoryComponent.h"
#include"../WarriorCharacter.h"
#include "HUD/InventorySlotWidget.h"



void UInventoryWidget::NativeConstruct()
{
}



void UInventoryWidget::SetDefenseText(int32 DefensePoint)
{
   const FString DPString = FString::Printf(TEXT("Defense Point :  %d"),DefensePoint);
   const FText DPText = FText::FromString(DPString);
   DefensePointText->SetText(DPText);
}

void UInventoryWidget::CheckHoveringOnInventoryList(const FDragDropEvent& InDragDropEvent)
{
	if (InventoryList)
	{
		FGeometry InventoryListGeometry = InventoryList->GetCachedGeometry();

		FVector2D MousePosition = InDragDropEvent.GetScreenSpacePosition();

		if (InventoryListGeometry.IsUnderLocation(MousePosition))
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString(TEXT("Hovering")));

			bIsHovering = true;
		}
		else
		{
			bIsHovering = false;
		}

	}
}

bool UInventoryWidget::IsHoveringOnInventoryList()
{
	return bIsHovering == true;
}




void UInventoryWidget::SwapItems(int32 FromIndex, int32 ToIndex)
{
    AWarriorCharacter* Warrior = Cast<AWarriorCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
    if (Warrior && Warrior->GetInventoryComponent())
    {
        Warrior->GetInventoryComponent()->SwapInventoryItems(FromIndex, ToIndex);
    }
}


void UInventoryWidget::SaveSlotIndices()
{
    StoredSlotIndices.Empty();
	AWarriorCharacter* Warrior = Cast<AWarriorCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	const TArray<FInventoryStruct>& InventoryItems = Warrior->GetInventoryComponent()->InventoryItems;
    if (Warrior && Warrior->GetInventoryComponent())
    {
        StoredSlotIndices = Warrior->GetInventoryComponent()->SavedSlotIndices;
        if (StoredSlotIndices.Num() != 20)
        {
            StoredSlotIndices.SetNumUninitialized(20);
            for (int32 i = 0; i < 20; ++i)
            { 
                StoredSlotIndices[i] = i;
            }
        }
    }
	
}


void UInventoryWidget::UpdateInventoryDisplay(const TArray<FInventoryStruct>& InventoryItems)
{
    
    // 1. Null Check
    if (!InventoryList || !InventorySlotWidgetClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("InventoryList or SlotWidgetClass is null!"));
        return;
    }

    // 2. Temizlik
    InventoryList->ClearChildren();
    InventorySlots.Empty();

    // 3. 20 Slot için döngü (7 sütunlu grid varsayımı)
    for (int32 i = 0; i < 30; ++i)
    {
        // 4. Yeni Slot Widget Oluştur (DEĞİŞKEN ADI: SlotWidget)
        UInventorySlotWidget* SlotWidget = CreateWidget<UInventorySlotWidget>(this, InventorySlotWidgetClass);
        if (SlotWidget)
        {
            // 5. Slot Index Atama
              SlotWidget->SetSlotIndex(i);
              SlotWidget->ItemIndex = i;

            // 6. Item Varsa Görseli Güncelle
            if (InventoryItems.IsValidIndex(i))
            {
                SlotWidget->SetUp(InventoryItems[i]);
            }

            // 7. Grid'e Ekleme (7 sütunlu grid)
            if (UGridSlot* GridSlot = InventoryList->AddChildToGrid(SlotWidget, i / 6, i % 6))
            {
                GridSlot->SetPadding(FMargin(0.f,-30,0,-15)); 
                GridSlot->SetHorizontalAlignment(HAlign_Fill);
                GridSlot->SetVerticalAlignment(VAlign_Fill);
            }

            // 8. Slot'u Diziye Ekle
            
            InventorySlots.Add(SlotWidget);
            // Debug Log
            UE_LOG(LogTemp, Verbose, TEXT("Created slot %d with item: %s"),
                i,
                InventoryItems.IsValidIndex(i) ? *InventoryItems[i].ItemName : TEXT("Empty"));
        }
    }
}

FReply UInventoryWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	FKey PressedKey = InKeyEvent.GetKey();
	if (PressedKey == EKeys::I)
	{
		AWarriorCharacter* Warrior = Cast<AWarriorCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
		if (Warrior)
		{
			APlayerController* Controller = GetWorld()->GetFirstPlayerController();
			SaveSlotIndices();
			Warrior->GetInventoryComponent()->ToggleInventory(Controller);


		}
		return FReply::Handled();
	}

	return FReply::Unhandled();

}
