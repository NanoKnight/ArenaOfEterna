#include "Enemy/CombatDirector.h"
#include "Enemy/Enemy.h"
#include "../WarriorCharacter.h"

ACombatDirector::ACombatDirector()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ACombatDirector::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(
		UpdateCombatTimer,
		this,
		&ACombatDirector::UpdateCombat,
		CombatUpdateInterval,
		true
	);
}

void ACombatDirector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACombatDirector::RegisterEnemy(AEnemy* Enemy)
{
	if (!IsValid(Enemy) || Enemy->IsDead())
	{
		return;
	}

	RegisteredEnemies.AddUnique(Enemy);

	SelectAttacker();
	


}

void ACombatDirector::SelectAttacker()
{
	if (IsValid(CurrentAttacker) &&
		!CurrentAttacker->IsDead() &&
		IsValid(CurrentAttacker->GetCombatTarget()))
	{
		return;
	}

	CurrentAttacker = nullptr;


	float ClosestDistanceSquared = TNumericLimits<float>::Max();

	for (AEnemy* Enemy : RegisteredEnemies)
	{
		if (!IsValid(Enemy) || Enemy->IsDead())
		{
			continue;
		}

		AActor* Target = Enemy->GetCombatTarget();

		if (!IsValid(Target))
		{
			continue;
		}

		const float DistanceSquared = FVector::DistSquared(
			Enemy->GetActorLocation(),
			Target->GetActorLocation()
		);

		if (DistanceSquared < ClosestDistanceSquared)
		{
			ClosestDistanceSquared = DistanceSquared;
			if (CurrentAttacker == nullptr)
			{
				OldAttacker = Enemy;
				WarriorRef = Cast<AWarriorCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
				if (WarriorRef)
				{
					WarriorRef->SetCombatTarget(OldAttacker);

				}
			}

			CurrentAttacker = Enemy;
		}

		
	}
}

void ACombatDirector::SelectNextAttacker()
{

	int32 CurrentIndex = RegisteredEnemies.IndexOfByKey(CurrentAttacker);

	if (CurrentIndex != INDEX_NONE && RegisteredEnemies.Num() <= 1)return;

		int32 NextIndex = CurrentIndex + AttackerDirection;

		if (NextIndex >= RegisteredEnemies.Num())
		{
			AttackerDirection = -1;
			NextIndex = CurrentIndex - 1;
			

		}
		else if(NextIndex < 0)
		{
			AttackerDirection = 1 ;
			NextIndex = CurrentIndex +1;
		}
		CurrentAttacker = RegisteredEnemies[NextIndex];
		//CurrentAttacker->MoveToTarget(CurrentAttacker->GetCombatTarget());

}

void ACombatDirector::ReleaseAttacker(AEnemy* Enemy)
{
	if (!IsValid(Enemy))
	{
		return;
	}


	RegisteredEnemies.Remove(Enemy);

	if (CurrentAttacker == Enemy)
	{
		CurrentAttacker = nullptr;
	}

	SelectAttacker();
}

void ACombatDirector::ReleaseAttackPermission(AEnemy* Enemy)
{
	if (!IsValid(Enemy) || CurrentAttacker != Enemy)
	{
		return;
	}

	CurrentAttacker = nullptr;

	
	RegisteredEnemies.Remove(Enemy);

	if (!Enemy->IsDead() && IsValid(Enemy->GetCombatTarget()))
	{
		RegisteredEnemies.Add(Enemy);
	}

	SelectAttacker();
}

void ACombatDirector::UpdateCombat()
{

	for (AEnemy* Enemy : RegisteredEnemies)
	{
		
		/*
		FVector TargetLoc = Enemy->GetCombatTarget()->GetActorLocation() - 
			Enemy->GetCombatTarget()->GetActorForwardVector() * 300;
		if (Enemy != CurrentAttacker)
		{
			Enemy->MoveToSurroundLocation(TargetLoc);

		}*/
			
	}


	/*
	RegisteredEnemies.RemoveAll(
		[](AEnemy* Enemy)
		{
			return !IsValid(Enemy) || Enemy->IsDead();
		}
	);

	if (RegisteredEnemies.IsEmpty())
	{
		CurrentAttacker = nullptr;
		return;
	}

	SelectAttacker();

	SurroundAngleOffset +=
		SurroundRotationSpeed * CombatUpdateInterval;

	SurroundAngleOffset =
		FMath::Fmod(SurroundAngleOffset, 360.f);

	for (AEnemy* Enemy : RegisteredEnemies)
	{
		if (!IsValid(Enemy) ||
			Enemy->IsDead() ||
			Enemy == CurrentAttacker)
		{
			continue;
		}

		const FVector SurroundLocation = GetSurroundLocation(Enemy);

		if (!SurroundLocation.IsNearlyZero())
		{
			Enemy->MoveToSurroundLocation(SurroundLocation);
		}
	}*/



}

FVector ACombatDirector::GetSurroundLocation(AEnemy* Enemy) const
{
	if (!IsValid(Enemy) || RegisteredEnemies.IsEmpty())
	{
		return FVector::ZeroVector;
	}

	const int32 EnemyIndex =
		RegisteredEnemies.IndexOfByKey(Enemy);

	if (EnemyIndex == INDEX_NONE)
	{
		return FVector::ZeroVector;
	}

	AActor* Target = Enemy->GetCombatTarget();

	if (!IsValid(Target))
	{
		return FVector::ZeroVector;
	}

	const int32 EnemyCount =
		RegisteredEnemies.Num();

	const float AngleStep =
		360.f / static_cast<float>(EnemyCount);

	const float Angle =
		EnemyIndex * AngleStep + SurroundAngleOffset;

	const FVector Direction =
		FVector::ForwardVector.RotateAngleAxis(
			Angle,
			FVector::UpVector
		);

	FVector Result =
		Target->GetActorLocation() +
		Direction * SurroundRadius;

	Result.Z = Enemy->GetActorLocation().Z;

	return Result;
}