// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTX_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAttributeComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void RegenStamina(float DeltaTime);
	FTimerHandle Timer;
	FTimerHandle StaminaTimers;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:


	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float Stamina;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float MaxStamina;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float Health;

	UPROPERTY(EditAnywhere, category = "Actor Attributes")
	float MaxHealth;

	UPROPERTY(EditAnywhere, category = "Actor Attributes")
	float PotHealthValue;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float Defense;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float StaminaCost = 14;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float Shield;

	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float MaxShield;

	UPROPERTY(EditDefaultsOnly, Category = "Actor Attributes")
	float ShieldRegenRate = 5.f;

	UPROPERTY(EditDefaultsOnly, Category = "Actor Attributes")
	float StaminaRegenRate = 5.f;

	UPROPERTY(VisibleAnywhere, Category = "Actor Attributes")
	int32 Gold;
	int32 Level;
	UPROPERTY(EditAnywhere, Category = "Actor Attributes")
	float ExperiencePoint;
	float MaxExperiencePoint;





public:
	void ReciveDamage(float Damage);
	void ReciveStamina(float Damage);
	void ReciveShieldDamage(float damage);
	float HealthPercent();
	float StaminaPercent();
	float LevelBarPercent();
	void LevelUp();
	float ShieldPercent();
	bool IsAlive();
	bool IsShieldAlive();
	void RegenerateShield();
	void RegenerateStamina();
	void ShieldRegenerateTimer();
	void StaminaTimer();  
	void SetHealthBarPercent();
	void AddExperience(float NumberOfExperience);
	void AddGold(int32 GoldAmmount);
	void AddHealth(float HealthAmmount);


	/*   GETTER    */
	FORCEINLINE float GetHealth() const { return Health; }
	FORCEINLINE float GetPotHealth() const { return PotHealthValue; }
	FORCEINLINE float GetDefense() const { return Defense;}
	FORCEINLINE float GetExperience() const { return ExperiencePoint; }
	FORCEINLINE float GetMaxExperience() const {return MaxExperiencePoint;}
	FORCEINLINE float GetStaminaCost() const { return StaminaCost;}
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetStamina() const { return Stamina;}
	FORCEINLINE float GetMaxStamina() const { return MaxStamina; }


	FORCEINLINE int32 GetGold() const { return Gold; }
	FORCEINLINE int32 GetLevel() const { return Level; }


	/*   SETTER  */
	FORCEINLINE void SetHealth(float NewHealth) { Health = NewHealth; }
	FORCEINLINE void SetDefense(float NewDefense) { Defense = NewDefense; }
	FORCEINLINE void SetExp(float NewExp) { ExperiencePoint = NewExp; }
	FORCEINLINE void SetMaxExp(float NewMaxExp) { MaxExperiencePoint = NewMaxExp; }
	FORCEINLINE void SetStamina(float NewStamina) { Stamina = NewStamina; }
	FORCEINLINE void SetGold(int32 NewGold) { Gold = NewGold; }
	FORCEINLINE void SetLevel(int32 NewLevel) { Level = NewLevel; }


};
