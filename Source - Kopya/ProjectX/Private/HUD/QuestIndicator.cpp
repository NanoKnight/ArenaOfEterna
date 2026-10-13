// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/QuestIndicator.h"
#include "Kismet/GameplayStatics.h"
#include"Items\QuestActor.h"
#include "Blueprint/WidgetLayoutLibrary.h" 
#include"Components\Image.h"
#include "Components/CanvasPanelSlot.h"

void UQuestIndicator::UpdateIndicator()
{
    if (!QuestActor || !Icon) return;

    APlayerController* PC = GetOwningPlayer();
    if (!PC || !PC->PlayerCameraManager) return;

    FVector2D ScreenSize = UWidgetLayoutLibrary::GetViewportSize(this);
    float Scale = UWidgetLayoutLibrary::GetViewportScale(this);
    ScreenSize /= Scale;
    FVector2D ScreenCenter = ScreenSize * 0.5f;

    FVector CameraLoc = PC->PlayerCameraManager->GetCameraLocation();
    FRotator CameraRot = PC->PlayerCameraManager->GetCameraRotation();
    FVector TargetLoc = QuestActor->GetActorLocation();

    FVector RelativePos = CameraRot.UnrotateVector(TargetLoc - CameraLoc);

    FVector2D ScreenPosition;
    bool bProjected = UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(PC, TargetLoc, ScreenPosition, false);

    float Margin = 90.0f;
    bool bIsInFront = (RelativePos.X > 0);
    bool bIsInsideBounds = (ScreenPosition.X > Margin && ScreenPosition.X < (ScreenSize.X - Margin)) &&
        (ScreenPosition.Y > Margin && ScreenPosition.Y < (ScreenSize.Y - Margin));
    bool bIsOnScreen = bProjected && bIsInFront && bIsInsideBounds;

    bool bIsInsideX = (ScreenPosition.X > Margin && ScreenPosition.X < (ScreenSize.X - Margin));
    bool bIsInsideY = (ScreenPosition.Y > Margin && ScreenPosition.Y < (ScreenSize.Y - Margin));

    if (!bIsInFront || !bProjected || !bIsInsideX || !bIsInsideY)
    {
        FVector2D Direction;

        if (!bIsInFront)
        {
          
            Direction = FVector2D(RelativePos.Y, -RelativePos.Z);
            if (Direction.SizeSquared() < 0.001f)
            {
                Direction = FVector2D(0.0f, 1.0f); 
            }
            Direction.Normalize();
        }
        else
        {
            
            Direction = (ScreenPosition - ScreenCenter).GetSafeNormal();
        }

        float PushDistance = ScreenSize.Size();
        ScreenPosition = ScreenCenter + (Direction * PushDistance);
    }

    ScreenPosition.X = FMath::Clamp(ScreenPosition.X, Margin, ScreenSize.X - Margin);
    ScreenPosition.Y = FMath::Clamp(ScreenPosition.Y, Margin, ScreenSize.Y - Margin);


    if (Icon->GetVisibility() != ESlateVisibility::HitTestInvisible)
    {
        Icon->SetVisibility(ESlateVisibility::HitTestInvisible);
    }

    UCanvasPanelSlot* IconSlot = Cast<UCanvasPanelSlot>(Icon->Slot);
    if (IconSlot)
    {
        IconSlot->SetPosition(ScreenPosition);
        Icon->SetVisibility(ESlateVisibility::Visible);


    }
    if (bIsOnScreen)
    {
        Icon->SetVisibility(ESlateVisibility::Hidden);
    }
}

void UQuestIndicator::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry,InDeltaTime);
	UpdateIndicator();
}

void UQuestIndicator::NativeConstruct()
{

	Super::NativeConstruct();
	GetWorld()->GetTimerManager().SetTimer(IndicatorTimer, this, &UQuestIndicator::SetQuestActor, 1.f);

  
	


}

void UQuestIndicator::SetQuestActor()
{
	QuestActor = Cast<AQuestActor>(UGameplayStatics::GetActorOfClass(GetWorld(), AQuestActor::StaticClass()));
}
