// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyAIController.h"
#include "Navigation/CrowdFollowingComponent.h"
#include"Enemy\Enemy.h"

AEnemyAIController::AEnemyAIController(const FObjectInitializer& ObjectInitializer)
	: Super(
		ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(
			TEXT("PathFollowingComponent")
		)
	)
{

}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	Enemy = Cast<AEnemy>(GetPawn());

	UCrowdFollowingComponent* CrowdComponent = Cast<UCrowdFollowingComponent>
		(GetPathFollowingComponent());
	if (!CrowdComponent)
	{
		return;
	}

	CrowdComponent->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::High);
	CrowdComponent->SetCrowdSeparation(true);
	CrowdComponent->SetCrowdSeparationWeight(2.f);


}
