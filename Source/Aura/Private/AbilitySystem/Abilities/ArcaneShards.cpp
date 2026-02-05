// Copyright Salmas Corporation


#include "AbilitySystem/Abilities/ArcaneShards.h"

FString UArcaneShards::GetDescription(int32 Level)
{
	const int32 NumberOfShards = FMath::Min(Level, MaxNumShards);
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = GetManaCost(Level);
	const float Cooldown = GetCooldown(Level);

	if (NumberOfShards == 1)
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>ARCANE SHARDS</>\n\n"

			// Details
			"<Small>Level: </><Level>%i</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			// Description
			"<Default>Spawns %i arcane shard, "
			"launching the target and dealing:</> "
			"<Damage>%i</><Default> arcane damage</>"),
			Level, abs(ManaCost), Cooldown, NumberOfShards, ScaledDamage);
	}
	
	return FString::Printf(TEXT(
		// Title
		"<Title>ARCANE SHARDS</>\n\n"

		// Details
		"<Small>Level: </><Level>%i</>\n"
		"<Small>ManaCost: </><ManaCost>%.1f</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

		// Description
		"<Default>Spawns %i arcane shards, "
		"launching the target and dealing:</> "
		"<Damage>%i</><Default> arcane damage</>"),
		Level, abs(ManaCost), Cooldown, NumberOfShards, ScaledDamage);
}

FString UArcaneShards::GetNextLevelDescription(int32 Level)
{
	// const int32 NumberOfShards = Level <=5 ? Level : 5;
	const int32 NumberOfShards = 1;
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost = GetManaCost(Level);
	const float Cooldown = GetCooldown(Level);

	if (NumberOfShards == 1)
	{
		return FString::Printf(TEXT(
			// Title
			"<Title>NEXT LEVEL</>\n\n"

			// Details
			"<Small>Level: </><Level>%i</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			// Description
			"<Default>Spawns %i arcane shard, "
			"launching the target and dealing:</> "
			"<Damage>%i</><Default> arcane damage</>"),
			Level, abs(ManaCost), Cooldown, NumberOfShards, ScaledDamage);
	}
	
	return FString::Printf(TEXT(
		// Title
		"<Title>NEXT LEVEL</>\n\n"

		// Details
		"<Small>Level: </><Level>%i</>\n"
		"<Small>ManaCost: </><ManaCost>%.1f</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

		// Description
		"<Default>Spawns %i arcane shards, "
		"launching the target and dealing:</> "
		"<Damage>%i</><Default> arcane damage</>"),
		Level, abs(ManaCost), Cooldown, NumberOfShards, ScaledDamage);
}
