// Copyright Salmas Corporation


#include "AbilitySystem/Abilities/AuraGameplayAbility.h"

#include "AbilitySystem/AuraAttributeSet.h"

FString UAuraGameplayAbility::GetDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>%s, </><Level>%d</>"), L"Default Ability Name - Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum." , Level);
}

FString UAuraGameplayAbility::GetNextLevelDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>Next Level: </><Level>%d</> \n<Default>Causes much more damage </>"), Level);
}

FString UAuraGameplayAbility::GetLockedDescription(int32 Level)
{
	return FString::Printf(TEXT("<Default>Spell Locked Until Level: %d </>"), Level);
}

float UAuraGameplayAbility::GetManaCost(float InLevel) const
{
	float ManaCost = 0.0f;
	if (const UGameplayEffect* CostEffect = GetCostGameplayEffect())
	{
		for (FGameplayModifierInfo Mod : CostEffect->Modifiers)
		{
			if (Mod.Attribute == UAuraAttributeSet::GetManaAttribute())
			{
				Mod.ModifierMagnitude.GetStaticMagnitudeIfPossible(InLevel, ManaCost);
				break;
			}
		}
	}
	return ManaCost;
}

float UAuraGameplayAbility::GetCooldown(float InLevel) const
{
	if (!AbilityTuningRow.IsNull())
	{
		if (const FAbilityTuningRow* TuningRow =
			GetAbilityTuningRow())
		{
			return TuningRow->Cooldown.GetValueAtLevel(InLevel);
		}
	}

	// Legacy fallback for abilities not yet migrated.
	float Cooldown = 0.f;

	if (const UGameplayEffect* CooldownEffect =
		GetCooldownGameplayEffect())
	{
		CooldownEffect->DurationMagnitude
			.GetStaticMagnitudeIfPossible(InLevel, Cooldown);
	}

	return Cooldown;
}

void UAuraGameplayAbility::ApplyCooldown(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	/*
	 * Preserve the standard GAS behavior for abilities that have not
	 * been migrated to DT_AbilityTuning.
	 */
	if (AbilityTuningRow.IsNull())
	{
		Super::ApplyCooldown(Handle, ActorInfo, ActivationInfo);
		return;
	}

	const FAbilityTuningRow* TuningRow = GetAbilityTuningRow();

	if (!TuningRow)
	{
		Super::ApplyCooldown(Handle, ActorInfo, ActivationInfo);
		return;
	}

	const UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect();

	if (!CooldownEffect)
	{
		return;
	}

	const int32 AbilityLevel = GetAbilityLevel(Handle, ActorInfo);

	const float CooldownDuration = TuningRow->Cooldown.GetValueAtLevel(AbilityLevel);

	/*
	 * A zero cooldown means that no persistent cooldown effect
	 * needs to be applied.
	 */
	if (CooldownDuration <= 0.f)
	{
		return;
	}

	FGameplayEffectSpecHandle SpecHandle =
		MakeOutgoingGameplayEffectSpec(Handle, ActorInfo, ActivationInfo, CooldownEffect->GetClass(), AbilityLevel);

	if (!SpecHandle.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("%s failed to create its cooldown Gameplay Effect spec."), *GetNameSafe(this));

		return;
	}

	SpecHandle.Data->SetDuration(CooldownDuration, true);

	if (const FGameplayTagContainer* CooldownTags = GetCooldownTags())
	{
		SpecHandle.Data->DynamicGrantedTags.AppendTags(*CooldownTags);
	}

	ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo,SpecHandle);
}

const FAbilityTuningRow* UAuraGameplayAbility::GetAbilityTuningRow() const
{
	if (AbilityTuningRow.IsNull())
	{
		UE_LOG(
			LogTemp,
			Error,
			TEXT("%s has no Ability Tuning row assigned."),
			*GetNameSafe(this));

		return nullptr;
	}

	return AbilityTuningRow.GetRow<FAbilityTuningRow>(
		TEXT("UAuraGameplayAbility::GetAbilityTuningRow"));
}

void UAuraGameplayAbility::LogAbilityTuningValues() const
{
	const FAbilityTuningRow* TuningRow = GetAbilityTuningRow();

	if (!TuningRow)
	{
		return;
	}

	const int32 AbilityLevel = GetAbilityLevel();

	const float DamageVal =
		TuningRow->Damage.GetValueAtLevel(AbilityLevel);

	const float ManaCost =
		TuningRow->ManaCost.GetValueAtLevel(AbilityLevel);

	const float Cooldown =
		TuningRow->Cooldown.GetValueAtLevel(AbilityLevel);

	const float DebuffChanceVal =
		TuningRow->DebuffChance.GetValueAtLevel(AbilityLevel);

	const float KnockbackChance =
		TuningRow->KnockbackChance.GetValueAtLevel(AbilityLevel);
	
	const float StunChance =
		TuningRow->StunChance.GetValueAtLevel(AbilityLevel);
	
	const float SlowChance =
		TuningRow->SlowChance.GetValueAtLevel(AbilityLevel);

	UE_LOG(
		LogTemp,
		Warning,
		TEXT(
			"%s | Level: %d | Damage: %.2f | Mana Cost: %.2f | "
			"Cooldown: %.2f | Debuff Chance: %.2f | "
			"Knockback Chance: %.2f | Stun Chance: %.2f | Slow Chance: %.2f"),
		*GetNameSafe(this),
		AbilityLevel,
		DamageVal,
		ManaCost,
		Cooldown,
		DebuffChanceVal,
		KnockbackChance,
		StunChance,
		SlowChance
		);
}
