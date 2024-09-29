// Fill out your copyright notice in the Description page of Project Settings.


#include "WarriorAnimClass.h"
#include "ProjectX/WarriorCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UWarriorAnimClass::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	WarriroCharacter = Cast<AWarriorCharacter>(TryGetPawnOwner());

	if (WarriroCharacter)
	{
		WarriorMovement = WarriroCharacter->GetCharacterMovement();
	}

	
}

void UWarriorAnimClass::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	if (WarriorMovement)
	{

	
	 GroundSpeed  = UKismetMathLibrary::VSizeXY(WarriorMovement->Velocity);
	 IsFalling = WarriorMovement->IsFalling();
	 CharacterStates = WarriroCharacter->GetCharacterStates();
	 ActionState = WarriroCharacter->GetActionState();
	 DeathPose = WarriroCharacter->GetDeathPose();
	}

}
