#include "Enemy/CombatDirector.h"
#include "Enemy/Enemy.h"
#include "../WarriorCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

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

	SelectAttackers();
}

void ACombatDirector::SelectAttackers()
{
	RegisteredEnemies.RemoveAll(
		[](AEnemy* Enemy)
		{
			return !IsValid(Enemy) ||
				Enemy->IsDead() ||
				!IsValid(Enemy->GetCombatTarget());
		}
	);

	if (RegisteredEnemies.IsEmpty())
	{
		ActiveAttackers.Empty();
		nextAttackerIndex = 0;
		return;
	}

	ActiveAttackers.RemoveAll(
		[this](AEnemy* Enemy)
		{
			return !IsValid(Enemy) ||
				Enemy->IsDead() ||
				!RegisteredEnemies.Contains(Enemy) ||
				!IsValid(Enemy->GetCombatTarget());
		}
	);

	// Hâlâ saldýran biri varsa yeni tur baþlatma.
	if (!ActiveAttackers.IsEmpty())
	{
		return;
	}

	int32 AttackCount = 1;

	// Normalde 1 kiþi, bazen 2 kiþi saldýrýr.
	if (RegisteredEnemies.Num() >= 2 &&
		FMath::FRand() <= DoubleAttackChance)
	{
		AttackCount = 2;
	}

	AttackCount = FMath::Min(AttackCount, MaxAttackers);
	AttackCount = FMath::Min(AttackCount, RegisteredEnemies.Num());

	for (int32 i = 0; i < AttackCount; i++)
	{
		if (RegisteredEnemies.IsEmpty())
		{
			break;
		}

		if (nextAttackerIndex >= RegisteredEnemies.Num())
		{
			nextAttackerIndex = 0;
		}

		AEnemy* Enemy = RegisteredEnemies[nextAttackerIndex];

		if (IsValid(Enemy) &&
			!Enemy->IsDead() &&
			!ActiveAttackers.Contains(Enemy))
		{
			ActiveAttackers.Add(Enemy);
		}

		nextAttackerIndex++;
	}
}

void ACombatDirector::ReleaseAttackSlot(AEnemy* Enemy)
{
	if (!IsValid(Enemy))
	{
		return;
	}

	ActiveAttackers.Remove(Enemy);

	// Ayný turda iki attacker varsa,
	// diðer attacker'ýn saldýrýsýnýn bitmesini bekle.
	if (!ActiveAttackers.IsEmpty())
	{
		return;
	}

	SelectAttackers();
}

void ACombatDirector::ReleaseAttacker(AEnemy* Enemy)
{
	if (!IsValid(Enemy))
	{
		return;
	}

	RegisteredEnemies.Remove(Enemy);
	ActiveAttackers.Remove(Enemy);

	if (nextAttackerIndex >= RegisteredEnemies.Num())
	{
		nextAttackerIndex = 0;
	}

	SelectAttackers();
}

bool ACombatDirector::HasAttackPermission(const AEnemy* Enemy) const
{
	return IsValid(Enemy) &&
		ActiveAttackers.Contains(Enemy);
}

void ACombatDirector::UpdateCombat()
{


	RegisteredEnemies.RemoveAll(
		[](AEnemy* Enemy)
		{
			return !IsValid(Enemy) ||
				Enemy->IsDead() ||
				!IsValid(Enemy->GetCombatTarget());
		}
	);

	ActiveAttackers.RemoveAll(
		[this](AEnemy* Enemy)
		{
			return !IsValid(Enemy) ||
				Enemy->IsDead() ||
				!RegisteredEnemies.Contains(Enemy) ||
				!IsValid(Enemy->GetCombatTarget());
		}
	);

	if (RegisteredEnemies.IsEmpty())
	{
		ActiveAttackers.Empty();
		nextAttackerIndex = 0;
		return;
	}

	SelectAttackers();

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

		if (ActiveAttackers.Contains(Enemy))
		{
			Enemy->GetCharacterMovement()->bOrientRotationToMovement = true;
		}
		else
		{
			Enemy->GetCharacterMovement()->bOrientRotationToMovement = false;


			FVector SurroundLocation = GetSurroundLocation(Enemy);

			if (!SurroundLocation.IsNearlyZero())
			{
				Enemy->MoveToSurroundLocation(SurroundLocation);
			}
		}
		
		
	}
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

	if (EnemyCount <= 0)
	{
		return FVector::ZeroVector;
	}

	const float AngleStep =
		360.f / static_cast<float>(EnemyCount);

	const float Angle = 	EnemyIndex * AngleStep;

	const FVector Direction = FVector::ForwardVector.RotateAngleAxis
	(Angle,FVector::UpVector);

	FVector Result = Target->GetActorLocation() + Direction * SurroundRadius;

	Result.Z = Enemy->GetActorLocation().Z;

	return Result;
}