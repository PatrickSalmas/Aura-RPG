// Copyright Salmas Corporation


#include "AbilitySystem/Abilities/Electrocute.h"

FString UElectrocute::GetDescription(int32 Level)
{
	const int32 NumberOfTargets = FMath::Min(Level, MaxNumShockTargets);
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = GetManaCost(Level);
	const float Cooldown = GetCooldown(Level);

	if (NumberOfTargets == 1)
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>ELECTROCUTE</>\n\n"

			// Details
			"<Small>Level: </><Level>%d</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			// Description
			"<Default>Emits a beam of lightning, "
			"connecting with the target, repeatedly causing </> "
			"<Damage>%d</><Default> lightning damage with a chance to stun</>"),
			Level, abs(ManaCost), Cooldown, ScaledDamage);
	}
	
	return FString::Printf(TEXT(
		// Title
		"<Title>ELECTROCUTE</>\n\n"

		// Details
		"<Small>Level: </><Level>%i</>\n"
		"<Small>ManaCost: </><ManaCost>%.1f</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

		// Description
		"<Default>Emits a beam of lightning, "
		"propagating to %d additional enemies nearby, causing :</> "
		"<Damage>%d</><Default> lightning damage with a chance to stun</>"),
		Level, abs(ManaCost), Cooldown, NumberOfTargets - 1, ScaledDamage);
}

FString UElectrocute::GetNextLevelDescription(int32 Level)
{
	const int32 NumberOfTargets = FMath::Min(Level, MaxNumShockTargets);
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = GetManaCost(Level);
	const float Cooldown = GetCooldown(Level);

	if (NumberOfTargets == 1)
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>ELECTROCUTE</>\n\n"

			// Details
			"<Small>Level: </><Level>%d</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			// Description
			"<Default>Emits a beam of lightning, "
			"connecting with the target, repeatedly causing </> "
			"<Damage>%d</><Default> lightning damage with a chance to stun</>"),
			Level, abs(ManaCost), Cooldown, ScaledDamage);
	}
	
	return FString::Printf(TEXT(
		// Title
		"<Title>ELECTROCUTE</>\n\n"

		// Details
		"<Small>Level: </><Level>%i</>\n"
		"<Small>ManaCost: </><ManaCost>%.1f</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

		// Description
		"<Default>Emits a beam of lightning, "
		"propagating to %d additional enemies nearby, causing :</> "
		"<Damage>%d</><Default> lightning damage with a chance to stun</>"),
		Level, abs(ManaCost), Cooldown, NumberOfTargets - 1, ScaledDamage);
}
