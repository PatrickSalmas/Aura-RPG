// Copyright Salmas Corporation


#include "AbilitySystem/Abilities/AuraFireBolt.h"

FString UAuraFireBolt::GetDescription(int32 Level)
{
	const int32 NumberOfProjectiles = Level <=5 ? Level : 5;
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = GetManaCost(Level);
	const float Cooldown = GetCooldown(Level);

	if (NumberOfProjectiles == 1)
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>FIREBOLT</>\n\n"

			// Details
			"<Small>Level: </><Level>%i</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			// Description
			"<Default>Launches %i bolt of fire, "
			"exploding on impact and dealing:</> "
			"<Damage>%i</><Default> fire damage with a chance to burn </>"),
			Level, abs(ManaCost), Cooldown, NumberOfProjectiles, ScaledDamage);
	}
	
	return FString::Printf(TEXT(
		// Title
		"<Title>FIREBOLT</>\n\n"

		// Details
		"<Small>Level: </><Level>%i</>\n"
		"<Small>ManaCost: </><ManaCost>%.1f</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

		// Description
		"<Default>Launches %i bolts of fire, "
		"exploding on impact and dealing:</> "
		"<Damage>%i</><Default> fire damage with a chance to burn </>"),
		Level, abs(ManaCost), Cooldown, NumberOfProjectiles, ScaledDamage);
}

FString UAuraFireBolt::GetNextLevelDescription(int32 Level)
{
	const int32 NumberOfProjectiles = Level <=5 ? Level : 5;
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = GetManaCost(Level);
	const float Cooldown = GetCooldown(Level);
	
	if (NumberOfProjectiles == 1)
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>NEXT LEVEL</>\n\n"

			// Details
			"<Small>Level: </><Level>%i</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			// Description
			"<Default>Launches %i bolt of fire, "
			"exploding on impact and dealing:</> "
			"<Damage>%i</><Default> fire damage with a chance to burn </>"),
			Level, abs(ManaCost), Cooldown, NumberOfProjectiles, ScaledDamage);
	}

	return FString::Printf(TEXT(
		// Title
		"<Title>NEXT LEVEL</>\n\n"

		// Details
		"<Small>Level: </><Level>%i</>\n"
		"<Small>ManaCost: </><ManaCost>%.1f</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

		// Description
		"<Default>Launches %i bolts of fire, "
		"exploding on impact and dealing:</> "
		"<Damage>%i</><Default> fire damage with a chance to burn </>"),
		Level, abs(ManaCost), Cooldown, NumberOfProjectiles, ScaledDamage);
}