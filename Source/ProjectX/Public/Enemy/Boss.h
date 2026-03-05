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
class UCurrentBossOverlayWidget;

UCLASS()
class PROJECTX_API ABoss : public AEnemy , public ICombatStateInterFace 
{
	GENERATED_BODY()

    protected:
		virtual void BeginPlay() override;
		virtual void Attack() override;

		virtual void AttackEnd() override;


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

		virtual  void ChaseTarget() override;
		virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
		virtual void Die()override;
		void SetBossHealthBar();

		UPROPERTY(EditDefaultsOnly)
		FString BossName;


	public:
	     ABoss();
		 void SetNoCollisionDamageForSpheres();
		virtual void Tick(float DeltaTime) override;
		virtual void SetEnemyState_Implementation(EEnemyState NewState) override;

		UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> OptionalPortalClass;

	 

	private:
	   void	SphereTrace(USphereComponent* SphereRef,FHitResult& TraceHit);


	   UPROPERTY(EditAnywhere)
	   TSubclassOf<UCurrentBossOverlayWidget> BossOverlayClass;

	   UCurrentBossOverlayWidget* BossOverlay;

	   bool OverlayWidgetCreated = false;
	   bool StaminaTimerStarted = false;

		TArray <AActor*> IgnoreActors;

};
