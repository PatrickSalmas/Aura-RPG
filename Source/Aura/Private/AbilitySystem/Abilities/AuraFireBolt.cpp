// Copyright Salmas Corporation


#include "AbilitySystem/Abilities/AuraFireBolt.h"
#include "Kismet/KismetSystemLibrary.h"

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

void UAuraFireBolt::SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag,
	bool bOverridePitch, float PitchOverride, AActor* HomingTarget)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(
		GetAvatarActorFromActorInfo(),
		SocketTag);
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	if (bOverridePitch) Rotation.Pitch = PitchOverride;
	
	const FVector Forward = Rotation.Vector();
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-ProjectileSpread / 2.f, FVector::UpVector);
	const FVector RightOfSpread = Forward.RotateAngleAxis(ProjectileSpread / 2.f, FVector::UpVector);

	
	// NumProjectiles = FMath::Min(MaxNumProjectiles, GetAbilityLevel());
	if (NumProjectiles > 1)
	{
		const float DeltaSpread = ProjectileSpread / (NumProjectiles - 1);
		for (int32 i = 0; i < NumProjectiles; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation,
									  SocketLocation + Direction * 75.f, 5, FLinearColor::Red, 120, 2);
		}
	}
	else
	{
		// Single Projectile
		UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation,
								  SocketLocation + Forward * 75.f, 5, FLinearColor::Red, 120, 2);
	}

	// UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation,
	//                                       SocketLocation + Forward * 100.f, 5, FLinearColor::White, 120, 2);
	//
	// UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation,
	// 								  SocketLocation + LeftOfSpread * 100.f, 5, FLinearColor::Green, 120, 2);
	//
	// UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation,
	// 							  SocketLocation + RightOfSpread * 100.f, 5, FLinearColor::Green, 120, 2);
}
