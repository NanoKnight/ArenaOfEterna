// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuestActor.generated.h"

class UWidgetComponent;
class UUserWidget;
UCLASS(Blueprintable)
class PROJECTX_API AQuestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AQuestActor();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* RootSceneComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Components")
	UWidgetComponent* QuestTracker;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void HiddenQuestTracker(bool TrackerState);

	bool playerIsRange() const;

	

private	:
	float DisplayDistance = 7000.f;


};
