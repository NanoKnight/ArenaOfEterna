#pragma once


UENUM(BlueprintType)
enum class ECharacterStates : uint8
{
	ECS_UnEquipped UMETA(DisplayName = "UnEquipped"),
	ECS_EquippedOnehand UMETA(DisplayName = "EquippedOnehand"),
	ECS_EquippedTwoHand UMETA(DisplayName = "EquippedTwohand"),
	ECS_EquippedShield UMETA(DisplayName = "EquippedShield")

};


UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAS_Unoccupied UMETA(DisplayName = "Unocuppied"),
	EAS_HitReaction UMETA(DisplayName = "HitReaction"),
	EAS_Attacking UMETA(DisplayName = "Attacking"),
	EAS_EquippingWeapon UMETA(DisplayName = "EquippingWeapon"),
	EAS_Dodge UMETA(DisplayName = "Dodge"),
	EAS_Dead UMETA(DisplayName = "Dead")
};


UENUM(BlueprintType)
enum  EDeadPose
{
	EDS_Dead1 UMETA(DisplayName = "Dead1"),
	EDS_Dead2 UMETA(DisplayName = "Dead2"),
	EDS_Dead3 UMETA(DisplayName = "Dead3"),
	EDS_Dead4 UMETA(DisplayName = "Dead4"),
	EDP_MAX UMETA(DisplayName = "DefaultMax")

};


UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	EES_Patrolling UMETA(DisplayName = "Patrolling"),
	EES_Chasing UMETA(DisplayName = "Chasing"),
	EES_Attacking UMETA(DisplayName = "Attacking"),
	EES_Engaged UMETA(DisplayName = "Engaged"),
	EES_NoState UMETA(DisplayName = "NoState"),
	EES_Dead UMETA(DisplayName = "Dead")


		
};