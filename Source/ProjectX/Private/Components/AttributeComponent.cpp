// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AttributeComponent.h"

// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	MaxExperiencePoint = 100;
	// ...
}


// Called when the game starts
void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


void UAttributeComponent::ShieldRegenerateTimer()
{
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &UAttributeComponent::RegenerateShield, 0.5f, false);

}

void UAttributeComponent::StaminaTimer()
{
	GetWorld()->GetTimerManager().SetTimer(StaminaTimers, this, &UAttributeComponent::RegenerateStamina, 0.5f, false);

}

void UAttributeComponent::AddExperience(float NumberOfExperience)
{
	ExperiencePoint += NumberOfExperience;
}

void UAttributeComponent::AddGold(int32 GoldAmmount)
{
	Gold += GoldAmmount;

}

void UAttributeComponent::AddHealth(float HealthAmmount)
{
	Health += HealthAmmount;
}



void UAttributeComponent::ReciveDamage(float Damage)
{
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
}

void UAttributeComponent::ReciveStamina(float Damage)
{
	Stamina = FMath::Clamp(Stamina - Damage, 0.f, MaxStamina);
}

void UAttributeComponent::ReciveShieldDamage(float damage)
{
	Shield = FMath::Clamp(Shield - damage, 0.f, MaxShield);
}



float UAttributeComponent::HealthPercent()
{
	return Health / MaxHealth;
}

float UAttributeComponent::StaminaPercent()
{
	return Stamina / MaxStamina;
}

float UAttributeComponent::LevelBarPercent()
{
	return ExperiencePoint / MaxExperiencePoint;
}

void UAttributeComponent::LevelUp()
{
	if (!GetOwner()->ActorHasTag("WarriorCharacter")) return;
	
	if (ExperiencePoint >= MaxExperiencePoint)
	{
		Level = ++ Level;
		ExperiencePoint = 0;
		MaxExperiencePoint += MaxExperiencePoint *0.5;
	}

}



float UAttributeComponent::ShieldPercent()
{
	return Shield / MaxShield;
}


bool UAttributeComponent::IsAlive()
{
	return Health > 0.f;
}

bool UAttributeComponent::IsShieldAlive()
{
	return Shield > 0.f;
}

void UAttributeComponent::RegenerateShield()
{
	if (Shield < MaxShield)
	{
		Shield += ShieldRegenRate;
		
		Shield = FMath::Clamp(Shield,0.f,MaxShield);
		ShieldRegenerateTimer();
		if (Shield >= MaxShield)
		{
			GetWorld()->GetTimerManager().ClearTimer(Timer);
			GEngine->AddOnScreenDebugMessage(1, 1.5, FColor::Red, TEXT("Health regeneration stopped"));
		}
	
		
	}
}

void UAttributeComponent::RegenerateStamina()
{
	if (Stamina < MaxShield)
	{

		Stamina += StaminaRegenRate;
		Stamina = FMath::Clamp(Stamina, 0.f, MaxShield);
		StaminaTimer();
		if (Stamina >= MaxStamina)
		{
			GetWorld()->GetTimerManager().ClearTimer(StaminaTimers);
		}


	}
}






// Called every frame
void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

}

void UAttributeComponent::RegenStamina(float DeltaTime)
{
	Stamina = FMath::Clamp(Stamina + StaminaRegenRate * DeltaTime, 0.f, MaxStamina);
}

