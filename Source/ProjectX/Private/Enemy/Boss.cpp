// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Boss.h"
#include"Components\AttributeComponent.h"
#include"Components\SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include"GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include"HUD\CurrentBossOverlayWidget.h"


void ABoss::ChaseTarget()
{
	if (EnemyState == EEnemyState::EES_Dead) return;
	Super::ChaseTarget();


	if (BossOverlayClass && OverlayWidgetCreated == false)
	{
		OverlayWidgetCreated = true;
		BossOverlay = CreateWidget<UCurrentBossOverlayWidget>(GetWorld(), BossOverlayClass);
		if (BossOverlay)
		{
			BossOverlay->AddToViewport();
			SetBossHealthBar();
			if (!BossName.IsEmpty()) BossOverlay->SetCurrentBossName(BossName);
		}

	}

}

float ABoss::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (EnemyState == EEnemyState::EAS_Stun)
	{
		Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
		SetBossHealthBar();

	}
	else
	{
		Super::TakeDamage(DamageAmount / 3, DamageEvent, EventInstigator, DamageCauser);

		SetBossHealthBar();
	}

		return DamageAmount;
	
	
}

void ABoss::Die()
{
	Super::Die();
	if (BossOverlay)
	{
		BossOverlay->RemoveFromParent();
		BossOverlay = nullptr;
	}
	

	GetWorld()->GetTimerManager().SetTimer(PortalTimer, this, &ABoss::SpawnPortal, 2.f);
}

void ABoss::SpawnPortal()
{
	if (OptionalPortalClass)
	{
		FRotator Rot(0, 0, 0);
		UWorld* World = GetWorld();
		AActor* Portal = World->SpawnActor<AActor>(OptionalPortalClass, OptionalPortalLocation, Rot);
	}
}

void ABoss::SetBossHealthBar()
{
	if (BossOverlay)
	{
		BossOverlay->SetCurrentBossHealthBar(Attributes->HealthPercent());

	}
}

ABoss::ABoss()
{
	DamageSphere1 = CreateDefaultSubobject<USphereComponent>("Damage Sphere 1");
	DamageSphere1->SetupAttachment(GetMesh());

	DamageSphere2 = CreateDefaultSubobject<USphereComponent>("Damage Sphere 2");
	DamageSphere2->SetupAttachment(GetMesh());


	DamageSphere3 = CreateDefaultSubobject<USphereComponent>("Damage Sphere 3");
	DamageSphere3->SetupAttachment(GetMesh());
	SetNoCollisionDamageForSpheres();



}

void ABoss::SetNoCollisionDamageForSpheres()
{

	DamageSphere1->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DamageSphere1->SetCollisionResponseToAllChannels(ECR_Overlap);
	DamageSphere1->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	DamageSphere2->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DamageSphere2->SetCollisionResponseToAllChannels(ECR_Overlap);
	DamageSphere2->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);


	DamageSphere3->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	DamageSphere3->SetCollisionResponseToAllChannels(ECR_Overlap);
	DamageSphere3->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
}

void ABoss::BeginPlay()
{
	Super::BeginPlay();
	DamageSphere1->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("LeftDamage"));
	DamageSphere2->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("RightDamage"));




	DamageSphere1->OnComponentBeginOverlap.AddDynamic(this, &ABoss::OnDamageSphereOneOverlap);
	DamageSphere2->OnComponentBeginOverlap.AddDynamic(this, &ABoss::OnDamageSphereTwoOverlap);



}

void ABoss::Attack()
{
	if (Attributes->GetStamina() >= 10 && EnemyState != EEnemyState::EES_Dead)
	{
		Super::Attack();


	}



}


void ABoss::AttackEnd()
{
	Super::AttackEnd();

	Attributes->ReciveStamina(Attributes->GetStaminaCost());
	SetNoCollisionDamageForSpheres();

	if (Attributes->GetStamina() <= 0)
	{
		EnemyState = EEnemyState::EAS_Stun;
		StartStaminaRegenerateTimer();
		SetNoCollisionDamageForSpheres();
		GetCharacterMovement()->StopMovementImmediately();

	}
}

void ABoss::SetEnableDamageCollision(USphereComponent* SphereRef, ECollisionEnabled::Type CollisionEnabled)
{
	if (SphereRef)
	{
		if (CollisionEnabled == ECollisionEnabled::NoCollision)
		{

			IgnoreActors.Empty();


		}
		SphereRef->SetCollisionEnabled(CollisionEnabled);
	}

}





void ABoss::OnDamageSphereOneOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{   


	CallDamageInterface(DamageSphere1);

}

void ABoss::OnDamageSphereTwoOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	CallDamageInterface(DamageSphere2);

}

void ABoss::OnDamageSphereThreeOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CallDamageInterface(DamageSphere3);

}

void ABoss::CallDamageInterface(USphereComponent* SphereRef)
{
	FHitResult SphereHit;
	SphereTrace(SphereRef, SphereHit);

	if (SphereHit.GetActor())
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *SphereHit.GetActor()->GetName());
		UGameplayStatics::ApplyDamage(SphereHit.GetActor(), 20.f, GetInstigator()->GetController(), this, UDamageType::StaticClass());
		ExecuteGetHit(SphereHit);
	}
}





void ABoss::SphereTrace(USphereComponent* SphereRef,FHitResult& TraceHit)
{
	if (!SphereRef) return;
	const FVector Start = SphereRef->GetComponentLocation();
	const FVector End = SphereRef->GetComponentLocation();

	TArray<AActor*> ActorsToIngnore;

	for (AActor* Actor : IgnoreActors)
	{

		ActorsToIngnore.AddUnique(Actor);
	}
	ActorsToIngnore.Add(GetOwner());

	ActorsToIngnore.Add(this);

	float SphereRadius = 32.f;
	ECollisionChannel TraceChannel = ECC_WorldDynamic;
	ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(TraceChannel);



	UKismetSystemLibrary::SphereTraceSingle
	(
		this,
		Start,
		End,
		SphereRadius,
		TraceType,
        false,
		IgnoreActors,
		EDrawDebugTrace::None,
		TraceHit,
		true);

	

	IgnoreActors.AddUnique(TraceHit.GetActor());
}


void ABoss::StartStaminaRegenerateTimer()
{
	if (StaminaTimerStarted == false)
	{
		StaminaTimerStarted = true;
		GetWorld()->GetTimerManager().SetTimer(StaminaRegenerateTimer, this, &ABoss::StaminaRegenerate, 3, false);
	}


}

void ABoss::StaminaRegenerate()
{
	if (EnemyState == EEnemyState::EES_Dead) return;
	
	Attributes->RegenerateStamina();

}

void ABoss::ExecuteGetHit(FHitResult& BoxHit)
{
	IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());
	if (HitInterface)
	{

		HitInterface->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint, GetOwner());

	}
	IgnoreActors.AddUnique(BoxHit.GetActor());
	CreateFields(BoxHit.ImpactPoint);
}



void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	


}

void ABoss::SetEnemyState_Implementation(EEnemyState NewState)
{
	EnemyState = NewState;
	StaminaTimerStarted = false;
	ChaseTarget();


}


