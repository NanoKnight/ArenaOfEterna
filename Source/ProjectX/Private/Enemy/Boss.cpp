// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Boss.h"
#include"Components\AttributeComponent.h"
#include"Components\SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"


ABoss::ABoss()
{
	DamageSphere1 = CreateDefaultSubobject<USphereComponent>("Damage Sphere 1");
	DamageSphere1->SetupAttachment(GetMesh());

	DamageSphere2 = CreateDefaultSubobject<USphereComponent>("Damage Sphere 2");
	DamageSphere2->SetupAttachment(GetMesh());


	DamageSphere3 = CreateDefaultSubobject<USphereComponent>("Damage Sphere 3");
	DamageSphere3->SetupAttachment(GetMesh());

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
	IgnoreActors.Empty();

	if (Attributes->GetStamina() >= 10)
	{
		Super::Attack();
		Attributes->ReciveStamina(10);

	}
	else if (Attributes->GetStamina() <= 0)
	{
		EnemyState = EEnemyState::EAS_Stun;
		StartStaminaRegenerateTimer();

	}
 
	



}

void ABoss::SetEnableDamageCollision(USphereComponent* SphereRef, ECollisionEnabled::Type CollisionEnabled)
{
	SphereRef->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	IgnoreActors.Empty();

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
		UGameplayStatics::ApplyDamage(SphereHit.GetActor(), 20, GetInstigator()->GetController(), this, UDamageType::StaticClass());
		ExecuteGetHit(SphereHit);
	}
}





void ABoss::SphereTrace(USphereComponent* SphereRef,FHitResult& TraceHit)
{
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
		EDrawDebugTrace::ForDuration,
		TraceHit,
		true);

	

	IgnoreActors.AddUnique(TraceHit.GetActor());
}


void ABoss::StartStaminaRegenerateTimer()
{
	GetWorld()->GetTimerManager().SetTimer(StaminaRegenerateTimer, this, &ABoss::StaminaRegenerate, 5, false);

}

void ABoss::StaminaRegenerate()
{
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


}


