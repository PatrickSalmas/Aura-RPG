// Copyright Salmas Coporation

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * AuraGameplayTags
 *
 * Singleton containing native Gameplay Tags
 */

struct FAuraGameplayTags
{
public:
	static const FAuraGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();

	FGameplayTag Actor_Player;
	FGameplayTag Actor_Enemy;
	
	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Resilience;
	FGameplayTag Attributes_Primary_Vigor;
	
	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_ArmorPenetration;
	FGameplayTag Attributes_Secondary_BlockChance;
	FGameplayTag Attributes_Secondary_CriticalHitChance;
	FGameplayTag Attributes_Secondary_CriticalHitDamage;
	FGameplayTag Attributes_Secondary_CriticalHitResistance;
	FGameplayTag Attributes_Secondary_HealthRegeneration;
	FGameplayTag Attributes_Secondary_ManaRegeneration;
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxMana;

	FGameplayTag Attributes_Meta_IncomingXP;
	
	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;
	FGameplayTag InputTag_PassiveAbility_1;
	FGameplayTag InputTag_PassiveAbility_2;

	FGameplayTag Damage;
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Lightning;
	FGameplayTag Damage_Arcane;
	FGameplayTag Damage_ArcaneImmobilize;
	FGameplayTag Damage_ArcaneSlow;
	FGameplayTag Damage_Physical;

	FGameplayTag Attributes_Resistance_Fire;
	FGameplayTag Attributes_Resistance_Lightning;
	FGameplayTag Attributes_Resistance_Arcane;
	FGameplayTag Attributes_Resistance_Physical;

	FGameplayTag Debuff_Burn;
	FGameplayTag Debuff_Stun;
	FGameplayTag Debuff_Charged;
	FGameplayTag Debuff_Burning;
	FGameplayTag Debuff_Exploitable;
	FGameplayTag Debuff_Immobilize;
	FGameplayTag Debuff_Slow;
	FGameplayTag Debuff_Arcane;
	FGameplayTag Debuff_Physical;

	FGameplayTag Debuff_Chance;
	FGameplayTag Debuff_Damage;
	FGameplayTag Debuff_Duration;
	FGameplayTag Debuff_Frequency;
	
	FGameplayTag ReactiveStatus_Chance;
	
	FGameplayTag Reaction_FireOnCharged;
	FGameplayTag Reaction_ArcaneOnCharged;
	FGameplayTag Reaction_LightningOnCharged;
	FGameplayTag Reaction_FireOnBurning;
	FGameplayTag Reaction_LightningOnBurning;
	FGameplayTag Reaction_ArcaneOnBurning;
	
	FGameplayTag Hit_ReactionInert;
	
	FGameplayTag Mechanic_Charged_Unlocked;

	FGameplayTag KnockBack_Chance;
	FGameplayTag KnockBack_ImpulseMagnitude;

	FGameplayTag Abilities_None;

	FGameplayTag Abilities_Attack;
	FGameplayTag Abilities_Summon;
	FGameplayTag Abilities_Buff;
	FGameplayTag Abilities_Physical_ChargeAttack;
	
	FGameplayTag Abilities_HitReact;

	FGameplayTag Abilities_Status_Locked;
	FGameplayTag Abilities_Status_Eligible;
	FGameplayTag Abilities_Status_Unlocked;
	FGameplayTag Abilities_Status_Equipped;

	FGameplayTag Abilities_Type_Offensive;
	FGameplayTag Abilities_Type_Passive;
	FGameplayTag Abilities_Type_None;
	
	FGameplayTag Abilities_Fire_FireBolt;
	FGameplayTag Abilities_Fire_FireBlast;
	FGameplayTag Abilities_Fire_FlameFlurry;
	FGameplayTag Abilities_Fire_FlameRupture;
	FGameplayTag Abilities_Fire_ShieldSurge;
	FGameplayTag Abilities_Fire_ShieldFlameThrower;
	FGameplayTag Abilities_Lightning_Electrocute;
	FGameplayTag Abilities_Lightning_ElectricSurge;
	FGameplayTag Abilities_Lightning_ElectricDomination;
	FGameplayTag Abilities_Lightning_LightningStep;
	FGameplayTag Abilities_Lightning_LightningNova;
	FGameplayTag Abilities_Arcane_ArcaneShards;
	FGameplayTag Abilities_Arcane_ArcaneExplosion;
	FGameplayTag Abilities_Arcane_ArcaneMine;
	FGameplayTag Abilities_Arcane_TimeDilation;
	
	FGameplayTag Abilities_Passive_HaloOfProtection;
	FGameplayTag Abilities_Passive_LifeSiphon;
	FGameplayTag Abilities_Passive_ManaSiphon;

	FGameplayTag Cooldown_Fire_FireBolt;
	FGameplayTag Cooldown_Fire_FireBlast;
	FGameplayTag Cooldown_Fire_FlameFlurry;
	FGameplayTag Cooldown_Fire_LavaCannonShot;
	FGameplayTag Cooldown_Lightning_Electrocute;
	FGameplayTag Cooldown_Arcane_ArcaneShards;
	FGameplayTag Cooldown_Buff_ApplySpeedToAllies;
	FGameplayTag Cooldown_ChargeAttack;
	
	FGameplayTag CombatSocket_Weapon;
	FGameplayTag CombatSocket_RightHand;
	FGameplayTag CombatSocket_LeftHand;
	FGameplayTag CombatSocket_Tail;
	FGameplayTag CombatSocket_LeftTentacle1;
	FGameplayTag CombatSocket_LeftTentacle2;
	FGameplayTag CombatSocket_RightTentacle1;
	FGameplayTag CombatSocket_RightTentacle2;
	
	FGameplayTag Montage_Attack_1;
	FGameplayTag Montage_Attack_2;
	FGameplayTag Montage_Attack_3;
	FGameplayTag Montage_Attack_4;

	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistances;
	TMap<FGameplayTag, FGameplayTag> DamageTypesToDebuffs;
	TMap<FGameplayTag, FGameplayTag> DamageTypesToReactiveStatuses;
	
	FGameplayTag SuccessfulReactiveStatus;
	
	FGameplayTag Effects_HitReact;

	FGameplayTag Player_Block_InputPressed;
	FGameplayTag Player_Block_InputHeld;
	FGameplayTag Player_Block_InputReleased;
	FGameplayTag Player_Block_CursorTrace;

	FGameplayTag GameplayCue_FireBlast;
	
protected:

private:
	static FAuraGameplayTags GameplayTags;
};
