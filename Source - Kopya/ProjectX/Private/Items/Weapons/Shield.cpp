// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapons/Shield.h"
#include "../WarriorCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components\SphereComponent.h"
#include "Components\BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include"Interfaces\HitInterface.h"
#include"NiagaraComponent.h"

AShield::AShield()
{
}

void AShield::Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator)
{
	SetOwner(NewOwner);
	SetInstigator(NewInstigator);
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(InParent, TransformRules, InSocketName);

}



