// Copyright Salmas Corporation


#include "AbilitySystem/Abilities/AuraFireBlast.h"

FString UAuraFireBlast::GetDescription(int32 Level)
{
	const int32 NumberOfProjectiles = Level <=5 ? Level : 5;
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = GetManaCost(Level);
	const float Cooldown = GetCooldown(Level);
	
	return FString::Printf(TEXT(
		// Title
		"<Title>FIRE BLAST</>\n\n"

		// Details
		"<Small>Level: </><Level>%i</>\n"
		"<Small>ManaCost: </><ManaCost>%.1f</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

		// Description
		"<Default>Launches %i balls of fire in all directions, then coming back and "
		"exploding on impact and dealing:</> "
		"<Damage>%i</><Default> fire damage with a chance to burn.</>"),
		NumFireballs, abs(ManaCost), Cooldown, NumberOfProjectiles, ScaledDamage);
}

FString UAuraFireBlast::GetNextLevelDescription(int32 Level)
{
	const int32 NumberOfProjectiles = Level <=5 ? Level : 5;
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = GetManaCost(Level);
	const float Cooldown = GetCooldown(Level);

	return FString::Printf(TEXT(
		// Title
		"<Title>NEXT LEVEL</>\n\n"

		// Details
		"<Small>Level: </><Level>%i</>\n"
		"<Small>ManaCost: </><ManaCost>%.1f</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

		// Description
		"<Default>Launches %i balls of fire in all directions, then coming back and "
		"exploding on impact and dealing:</> "
		"<Damage>%i</><Default> fire damage with a chance to burn.</>"),
		NumFireballs, abs(ManaCost), Cooldown, NumberOfProjectiles, ScaledDamage);
}