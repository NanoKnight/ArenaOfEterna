// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/Enemy.h"
#include"../Interfaces/CombatStateInterFace.h"
#include "../Interfaces/HitInterface.h"
#include "Boss.generated.h"

/**
 * 
 */

class USphereComponent;

UCLASS()
class PROJECTX_API ABoss : public AEnemy , public ICombatStateInterFace 
{
	GENERATED_BODY()

    protected:
		virtual void BeginPlay() override;
		virtual void Attack() override;

		UFUNCTION(BlueprintCallable) 
		void SetEnableDamageCollision(USphereComponent* SphereRef,ECollisionEnabled::Type CollisionEnabled);


		UFUNCTION()
		void OnDamageSphereOneOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
		
		UFUNCTION()
		void OnDamageSphereTwoOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
		
		UFUNCTION()
		void OnDamageSphereThreeOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
		
		void CallDamageInterface(USphereComponent* SphereRef );



		FTimerHandle StaminaRegenerateTimer;
		void StartStaminaRegenerateTimer();
		void StaminaRegenerate();

		void ExecuteGetHit(FHitResult& BoxHit);

		UFUNCTION(BlueprintImplementableEvent)
		void CreateFields(const FVector& FieldLocation);


		UPROPERTY(EditDefaultsOnly, BlueprintReadonly)
		USphereComponent* DamageSphere1;

		UPROPERTY(EditDefaultsOnly, BlueprintReadonly)
		USphereComponent* DamageSphere2;

		UPROPERTY(EditDefaultsOnly, BlueprintReadonly)
		USphereComponent* DamageSphere3;



	public:
	     ABoss();
		virtual void Tick(float DeltaTime) override;
		virtual void SetEnemyState_Implementation(EEnemyState NewState) override;



	 

	private:
	   void	SphereTrace(USphereComponent* SphereRef,FHitResult& TraceHit);
	



		TArray <AActor*> IgnoreActors;

};
