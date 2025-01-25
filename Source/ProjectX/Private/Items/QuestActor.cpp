// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/QuestActor.h"
#include "Kismet/GameplayStatics.h"
#include "./HUD/PlayerHUD.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include"../WarriorCharacter.h"
#include "Math/Vector2D.h" 
#include "Blueprint/UserWidget.h"
#include "Engine/Engine.h"  
#include"Components\WidgetComponent.h"

// Sets default values
AQuestActor::AQuestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = RootSceneComponent;
    
	QuestTracker = CreateDefaultSubobject<UWidgetComponent>(TEXT("QuestTracker"));
	QuestTracker->SetupAttachment(RootSceneComponent);
	

}

// Called when the game starts or when spawned
void AQuestActor::BeginPlay()
{
	Super::BeginPlay();

 
	
}

// Called every frame
void AQuestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (!playerIsRange())
    {
       
    }

}

void AQuestActor::HiddenQuestTracker(bool TrackerState)
{
	
		QuestTracker->SetVisibility(TrackerState);
	
}

bool AQuestActor::playerIsRange() const
{
	AWarriorCharacter* PlayerCharacter = Cast<AWarriorCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (PlayerCharacter)
	{
		float Distance = FVector::Dist(PlayerCharacter->GetActorLocation(), GetActorLocation());
		return Distance <= DisplayDistance;
	}
	return false;
}



