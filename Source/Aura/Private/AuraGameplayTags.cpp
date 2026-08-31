// Copyright Salmas Corporation


#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"

FAuraGameplayTags FAuraGameplayTags::GameplayTags;

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	/*
	 * Actor Tags
	 */
	GameplayTags.Actor_Player = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Actor.Player"), FString("Tag for the Player actor"));
	GameplayTags.Actor_Enemy = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Actor.Enemy"), FString("Tag for the Enemy actor"));
	
	/*
	 * Primary Attributes
	 */
	GameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Strength"), FString("Increases physical damage, Crit Chance, and Crit Damage"));
	GameplayTags.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Intelligence"), FString("Increases magical damage, Mana Regeneration, and Max Mana"));
	GameplayTags.Attributes_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Resilience"), FString("Increases Armor and Block Chance"));
	GameplayTags.Attributes_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Vigor"), FString("Increases Health, Health Regeneration, and Max Health"));
	
	/*
	 * Secondary Attributes
	 */
	GameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Armor"), FString("Reduces damage taken, improves Block Chance"));
	GameplayTags.Attributes_Secondary_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.ArmorPenetration"), FString("Ignores percentage of enemy Armor, increase Crit Hit Chance"));
	GameplayTags.Attributes_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.BlockChance"), FString("Chance to cut incoming damage in half"));
	GameplayTags.Attributes_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitChance"), FString("Chance to double damage plus critical hit bonus"));
	GameplayTags.Attributes_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitDamage"), FString("Bonus damage added when a critical hit is scored"));	GameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Armor"), FString("Reduces damage taken, improves Block Chance"));
	GameplayTags.Attributes_Secondary_CriticalHitResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitResistance"), FString("Reduces critical hit chance of attacking enemies"));
	GameplayTags.Attributes_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.HealthRegeneration"), FString("Amount of Health regenerated every 1 second"));
	GameplayTags.Attributes_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.ManaRegeneration"), FString("Amount of Mana regenerated every 1 second"));
	GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxHealth"), FString("Maximum amount of Health obtainable"));
	GameplayTags.Attributes_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxMana"), FString("Maximum amount of Mana obtainable"));

	/*
	 * Input Tags
	 */
	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.LMB"), FString("Input Tag for Left Mouse Button"));
	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.RMB"), FString("Input Tag for Right Mouse Button"));
	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.1"), FString("Input Tag for 1 key"));
	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.2"), FString("Input Tag for 2 key"));
	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.3"), FString("Input Tag for 3 key"));
	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.4"), FString("Input Tag for 4 key"));
	GameplayTags.InputTag_PassiveAbility_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.PassiveAbility1"), FString("Input Tag tag for Passive Ability 1"));
	GameplayTags.InputTag_PassiveAbility_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.PassiveAbility2"), FString("Input Tag tag for Passive Ability 2"));
	

	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage"), FString("Damage"));

	/*
	 * Damage Types
	 */
	GameplayTags.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Fire"), FString("Fire Damage Type"));
	GameplayTags.Damage_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Lightning"), FString("Lightning Damage Type"));
	GameplayTags.Damage_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Arcane"), FString("Arcane Damage Type"));
	GameplayTags.Damage_ArcaneFire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.ArcaneFire"), FString("ArcaneFire Damage Type"));
	GameplayTags.Damage_ArcaneImmobilize = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.ArcaneImmobilize"), FString("Arcane Damage Type"));
	GameplayTags.Damage_ArcaneSlow50 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.ArcaneSlow50"), FString("Arcane Slow Damage Type for applying 50% speed debuff"));
	GameplayTags.Damage_ArcaneSlow75 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.ArcaneSlow75"), FString("Arcane Slow Damage Type for applying 75% speed debuff"));
	GameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Physical"), FString("Physical Damage Type"));
	// GameplayTags.DamageTypes.Add(GameplayTags.Damage_Fire);

	/*
	 * Debuffs
	 */
	GameplayTags.Debuff_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Arcane"), FString("Debuff for Arcane Damage"));
	GameplayTags.Debuff_ArcaneFire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.ArcaneFire"), FString("Debuff for ArcaneFire Damage Effect"));
	// GameplayTags.Debuff_ArcaneSlow = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.ArcaneSlow"), FString("Debuff for Arcane Damage"));
	GameplayTags.Debuff_Burn = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Burn"), FString("Debuff for Fire Damage"));
	GameplayTags.Debuff_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Physical"), FString("Debuff for Physical Damage"));
	GameplayTags.Debuff_Stun = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Stun"), FString("Debuff for Lightning Damage"));
	GameplayTags.Debuff_Charged = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Charged"), FString("Exploitable Debuff for Lightning Damage"));
	GameplayTags.Debuff_Burning = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Burning"), FString("Exploitable Debuff for Fire Damage"));
	GameplayTags.Debuff_Unstable = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Unstable"), FString("Exploitable Debuff for Arcane Damage"));
	GameplayTags.Debuff_Exploitable = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Exploitable"), FString("Exploitable Debuff tag"));
	GameplayTags.Debuff_Immobilize = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Immobilize"), FString("Debuff for Arcane Damage"));
	GameplayTags.Debuff_Slow50 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Slow50"), FString("Debuff for Arcane Damage"));
	GameplayTags.Debuff_Slow75 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Slow75"), FString("Debuff for Arcane Damage"));
	GameplayTags.Debuff_Knockback = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Knockback"), FString("Debuff for Fire Damage"));
	
	GameplayTags.Debuff_Chance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Chance"), FString("Debuff Chance"));
	GameplayTags.Debuff_Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Damage"), FString("Debuff Damage"));
	GameplayTags.Debuff_Duration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Duration"), FString("Debuff Duration"));
	GameplayTags.Debuff_Frequency = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Frequency"), FString("Debuff Frequency"));
	
	GameplayTags.ReactiveStatus_Chance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("ReactiveStatus.Chance"), FString("Chance for a Reactive Status to be applied"));
	GameplayTags.ReactiveStatus_Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("ReactiveStatus.Damage"), FString("ReactiveStatus Damage"));
	GameplayTags.ReactiveStatus_Duration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("ReactiveStatus.Duration"), FString("ReactiveStatus Duration"));
	GameplayTags.ReactiveStatus_Frequency = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("ReactiveStatus.Frequency"), FString("ReactiveStatus Frequency"));
	
	GameplayTags.ReactiveStatus_Burning = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("ReactiveStatus.Burning"), FString("ReactiveStatus Burning"));
	GameplayTags.ReactiveStatus_Charged = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("ReactiveStatus.Charged"), FString("ReactiveStatus Charged"));
	GameplayTags.ReactiveStatus_Unstable = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("ReactiveStatus.Unstable"), FString("ReactiveStatus Unstable"));
	
	GameplayTags.Reaction_FireOnCharged = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Reaction.FireOnCharged"), FString("Fire on charged recation occurred"));
	GameplayTags.Reaction_ArcaneOnCharged = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Reaction.ArcaneOnCharged"), FString("Arcane on charged recation occurred"));
	GameplayTags.Reaction_LightningOnCharged = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Reaction.LightningOnCharged"), FString("Lightning on charged recation occurred"));
	GameplayTags.Reaction_FireOnBurning = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Reaction.FireOnBurning"), FString("Fire on burning recation occurred"));
	GameplayTags.Reaction_LightningOnBurning = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Reaction.LightningOnBurning"), FString("Lightning on burning recation occurred"));
	GameplayTags.Reaction_ArcaneOnBurning = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Reaction.ArcaneOnBurning"), FString("Arcane on burning recation occurred"));
	GameplayTags.Reaction_FireOnUnstable = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Reaction.FireOnUnstable"), FString("Fire on unstable recation occurred"));
	GameplayTags.Reaction_LightningOnUnstable = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Reaction.LightningOnUnstable"), FString("Lightning on unstable recation occurred"));
	GameplayTags.Reaction_ArcaneOnUnstable = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Reaction.ArcaneOnUnstable"), FString("Lightning on unstable recation occurred"));
	
	GameplayTags.Hit_ReactionInert = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Hit.ReactionInert"), FString("Tag to represent that hit should not cause hit react"));
	
	GameplayTags.Mechanic_Charged_Unlocked = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Mechanic.Charged.Unlocked"), FString("Tag for charged mechanic being unlocked"));
	
	/*
	 * Knockback, Stun, Slow
	 */
	GameplayTags.KnockBack_ImpulseMagnitude = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Knockback.ImpulseMagnitude"), FString("Knockback Impulse Magnitude"));
	GameplayTags.Stun_Chance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Stun.Chance"), FString("Stun Chance"));

	/*
	 * Resistances
	 */
	GameplayTags.Attributes_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Fire"), FString("Fire Resistance Type"));
	GameplayTags.Attributes_Resistance_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Lightning"), FString("Lightning Resistance Type"));
	GameplayTags.Attributes_Resistance_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Arcane"), FString("Arcane Resistance Type"));
	GameplayTags.Attributes_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Physical"), FString("Physical Resistance Type"));
	
	/*
	 * Meta Attributes
	 */
	GameplayTags.Attributes_Meta_IncomingXP = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Meta.IncomingXP"), FString("Incoming XP Meta Attribute"));


	/*
	 * Map of Damage Types to Resistances
	 */
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Fire, GameplayTags.Attributes_Resistance_Fire);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Lightning, GameplayTags.Attributes_Resistance_Lightning);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Arcane, GameplayTags.Attributes_Resistance_Arcane);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_ArcaneImmobilize, GameplayTags.Attributes_Resistance_Arcane);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_ArcaneSlow50, GameplayTags.Attributes_Resistance_Arcane);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_ArcaneSlow75, GameplayTags.Attributes_Resistance_Arcane);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_ArcaneFire, GameplayTags.Attributes_Resistance_Arcane);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Physical, GameplayTags.Attributes_Resistance_Physical);
	
	/*
	* Map of Damage Types to Debuffs
	*/
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Fire, GameplayTags.Debuff_Knockback);
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Lightning, GameplayTags.Debuff_Stun);
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_ArcaneFire ,GameplayTags.Debuff_ArcaneFire);
	// GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Arcane, GameplayTags.Debuff_Arcane);
	// GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Arcane, GameplayTags.Debuff_Immobilize);
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_ArcaneImmobilize, GameplayTags.Debuff_Immobilize);
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_ArcaneSlow50, GameplayTags.Debuff_Slow50);
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_ArcaneSlow75, GameplayTags.Debuff_Slow75);
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Physical, GameplayTags.Debuff_Physical);
	// GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Fire, GameplayTags.Debuff);
	
	
	GameplayTags.DamageTypesToReactiveStatuses.Add(GameplayTags.Damage_Lightning,GameplayTags.ReactiveStatus_Charged);
	GameplayTags.DamageTypesToReactiveStatuses.Add(GameplayTags.Damage_Fire,GameplayTags.ReactiveStatus_Burning);
	GameplayTags.DamageTypesToReactiveStatuses.Add(GameplayTags.Damage_Arcane,GameplayTags.ReactiveStatus_Unstable);
	GameplayTags.DamageTypesToReactiveStatuses.Add(GameplayTags.Damage_ArcaneSlow50,GameplayTags.ReactiveStatus_Unstable);
	GameplayTags.DamageTypesToReactiveStatuses.Add(GameplayTags.Damage_ArcaneSlow75,GameplayTags.ReactiveStatus_Unstable);
	GameplayTags.DamageTypesToReactiveStatuses.Add(GameplayTags.Damage_ArcaneImmobilize,GameplayTags.ReactiveStatus_Unstable);
	
	GameplayTags.SuccessfulReactiveStatus = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Reactive.SuccessfulReactiveStatus"), FString("Has Successful Reactive status"));
	
	/*
	 * Effects
	 */
	GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effects.HitReact"), FString("Tag granted when Hit Reacting"));

	/*
	 * Abilities
	 */
	GameplayTags.Abilities_None = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.None"), FString("No Ability - Like the nullptr for Ability Tags"));
	
	GameplayTags.Abilities_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Attack"), FString("Attack Ability Tag"));
	GameplayTags.Abilities_Summon = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Summon"), FString("Summon Ability Tag"));
	GameplayTags.Abilities_Buff = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Buff"), FString("Buff Ability Tag"));
	GameplayTags.Abilities_Physical_ChargeAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Physical.ChargeAttack"), FString("ChargeAttack Ability Tag"));

	/*
	* Offensive
	*/
	GameplayTags.Abilities_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Fire.FireBolt"), FString("FireBolt Ability Tag"));
	GameplayTags.Abilities_Fire_FireBlast = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Fire.FireBlast"), FString("FireBlast Ability Tag"));
	GameplayTags.Abilities_Fire_FlameFlurry = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Fire.FlameFlurry"), FString("FlameFlurry Ability Tag"));
	GameplayTags.Abilities_Fire_FlameRupture = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Fire.FlameRupture"), FString("FlameRupture Ability Tag"));
	GameplayTags.Abilities_Fire_ShieldSurge = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Fire.ShieldSurge"), FString("ShieldSurge Ability Tag"));
	GameplayTags.Abilities_Fire_ShieldFlameThrower = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Fire.ShieldFlameThrower"), FString("ShieldFlameThrower Ability Tag"));
	GameplayTags.Abilities_Lightning_LightningBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Lightning.LightningBolt"), FString("LightningBolt Ability Tag"));
	GameplayTags.Abilities_Lightning_Electrocute = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Lightning.Electrocute"), FString("Electrocute Ability Tag"));
	GameplayTags.Abilities_Lightning_ElectricSurge = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Lightning.ElectricSurge"), FString("Electric Surge Ability Tag"));
	GameplayTags.Abilities_Lightning_ElectricDomination = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Lightning.ElectricDomination"), FString("Electric Domination Ability Tag"));
	GameplayTags.Abilities_Lightning_LightningStep = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Lightning.LightningStep"), FString("Lightning Step Ability Tag"));
	GameplayTags.Abilities_Lightning_LightningNova = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Lightning.LightningNova"), FString("Lightning Nova Ability Tag"));
	GameplayTags.Abilities_Arcane_ArcaneShards = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Arcane.ArcaneShards"), FString("ArcaneShards Ability Tag"));
	GameplayTags.Abilities_Arcane_ArcaneExplosion = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Arcane.ArcaneExplosion"), FString("ArcaneExplosion Ability Tag"));
	GameplayTags.Abilities_Arcane_ArcaneMine = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Arcane.ArcaneMine"), FString("ArcaneMine Ability Tag"));
	GameplayTags.Abilities_Arcane_TimeDilation = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Arcane.TimeDilation"), FString("TimeDilation Ability Tag"));
	GameplayTags.Abilities_Hybrid_PhaseJolt = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Hybrid.PhaseJolt"), FString("PhaseJolt Ability Tag"));
	
	
	GameplayTags.Abilities_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.HitReact"), FString("Hit React Ability"));
	GameplayTags.Abilities_Status_Eligible = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.Eligible"), FString("Eligible Status"));
	GameplayTags.Abilities_Status_Equipped = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.Equipped"), FString("Equipped Status"));
	GameplayTags.Abilities_Status_Locked = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.Locked"), FString("Locked Status"));
	GameplayTags.Abilities_Status_Unlocked = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.Unlocked"), FString("Unlocked Status"));
	GameplayTags.Abilities_Type_Offensive = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Type.Offensive"), FString("Ability Type Offensive"));
	GameplayTags.Abilities_Type_Passive = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Type.Passive"), FString("Ability Type Passive"));
	GameplayTags.Abilities_Type_None = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Type.None"), FString("Ability Type None"));


	/*
	 * Passive Spells
	 */
	GameplayTags.Abilities_Passive_HaloOfProtection = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Passive.HaloOfProtection"), FString("HaloOfProtection"));
	GameplayTags.Abilities_Passive_LifeSiphon = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Passive.LifeSiphon"), FString("LifeSiphon"));
	GameplayTags.Abilities_Passive_ManaSiphon = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Passive.ManaSiphon"), FString("ManaSiphon"));
	
	
	/*
	 * Cooldowns
	 */
	GameplayTags.Cooldown_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Fire.FireBolt"), FString("FireBolt Cooldown Tag"));
	GameplayTags.Cooldown_Fire_FireBlast = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Fire.FireBlast"), FString("FireBlast Cooldown Tag"));
	GameplayTags.Cooldown_Fire_FlameFlurry= UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Fire.FlameFlurry"), FString("FlameFlurry Cooldown Tag"));
	GameplayTags.Cooldown_Fire_LavaCannonShot = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Fire.LavaCannonShot"), FString("LavaCannonShot Cooldown Tag"));
	GameplayTags.Cooldown_Lightning_Electrocute = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Lightning.Electrocute"), FString("Electrocute Cooldown Tag"));
	GameplayTags.Cooldown_Arcane_ArcaneShards = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Arcane.ArcaneShards"), FString("ArcaneShards Cooldown Tag"));
	GameplayTags.Cooldown_Buff_ApplySpeedToAllies = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Buff.ApplySpeedToAllies"), FString("ApplySpeedToAllies Cooldown Tag"));
	GameplayTags.Cooldown_ChargeAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Physical.ChargeAttack"), FString("ChargeAttack Cooldown Tag"));

	/*
	 * Combat Sockets
	 */
	GameplayTags.CombatSocket_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.Weapon"), FString("Weapon"));
	GameplayTags.CombatSocket_LeftHand = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.LeftHand"), FString("Left Hand"));
	GameplayTags.CombatSocket_RightHand = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.RightHand"), FString("Right Hand"));
	GameplayTags.CombatSocket_Tail = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.Tail"), FString("Tail"));
	GameplayTags.CombatSocket_LeftTentacle1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.LeftTentacle1"), FString("LeftTentacle1"));
	GameplayTags.CombatSocket_LeftTentacle2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.LeftTentacle2"), FString("LeftTentacle1"));
	GameplayTags.CombatSocket_RightTentacle1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.RightTentacle1"), FString("RightTentacle1"));
	GameplayTags.CombatSocket_RightTentacle2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.RightTentacle2"), FString("RightTentacle2"));

	/*
	 * Montage Tags
	 */
	GameplayTags.Montage_Attack_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.1"), FString("Attack 1"));
	GameplayTags.Montage_Attack_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.2"), FString("Attack 2"));
	GameplayTags.Montage_Attack_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.3"), FString("Attack 3"));
	GameplayTags.Montage_Attack_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.4"), FString("Attack 4"));

	/*
     * Player Tags
	 */
	GameplayTags.Player_Block_CursorTrace = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Player.Block.CursorTrace"), FString("Block Tracing under the cursor"));
	GameplayTags.Player_Block_InputHeld = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Player.Block.InputHeld"), FString("Block Input Held callback for input"));
	GameplayTags.Player_Block_InputPressed = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Player.Block.InputPressed"), FString("Block Input Pressed callback for input"));
	GameplayTags.Player_Block_InputReleased = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Player.Block.InputReleased"), FString("Block Input Released callback for input"));

	/*
	 * GameplayCues
	 */
	GameplayTags.GameplayCue_FireBlast = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("GameplayCue.FireBlast"), FString("FireBlast GameplayCue tag"));
	
}
