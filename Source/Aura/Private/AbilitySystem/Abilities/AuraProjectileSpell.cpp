// Copyright Salmas Corporation


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"
#include "LevelInstance/LevelInstanceTypes.h"
#include "Aura/Public/AuraGameplayTags.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo,
                                           const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag,
                                           bool bOverridePitch, float PitchOverride)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(
		GetAvatarActorFromActorInfo(),
		SocketTag);
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	if (bOverridePitch)
	{
		Rotation.Pitch = PitchOverride;
	}

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SocketLocation);
	SpawnTransform.SetRotation(Rotation.Quaternion());
		
	AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(ProjectileClass, SpawnTransform, GetOwningActorFromActorInfo(),
													Cast<APawn>(GetOwningActorFromActorInfo()),
													ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
	FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
	EffectContextHandle.SetAbility(this);
	EffectContextHandle.AddSourceObject(Projectile);
	TArray<TWeakObjectPtr<AActor>> Actors;
	Actors.Add(Projectile);
	EffectContextHandle.AddActors(Actors);
	FHitResult HitResult;
	HitResult.Location = ProjectileTargetLocation;
	EffectContextHandle.AddHitResult(HitResult);
		
	const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), EffectContextHandle);

	FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();

	for (auto& Pair : DamageTypes)
	{
		const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Pair.Key, ScaledDamage);
	}
		
	Projectile->DamageEffectSpecHandle = SpecHandle;
		
	Projectile->FinishSpawning(SpawnTransform);
}

FString UAuraProjectileSpell::GetDescription(int32 Level)
{
	const FString ProjectileBluePrint = ProjectileClass->GetName();
	FString ProjectileName = "";
	FString ElementType = "";
	const int32 NumberOfProjectiles = Level <=5 ? Level : 5;
	const int32 Damage = DamageTypes[FAuraGameplayTags::Get().Damage_Fire].GetValueAtLevel(Level);

	if (ProjectileBluePrint == "BP_FireBolt_C")
	{
		ProjectileName = "Fire Bolt";
		ElementType = "fire";
	}

	if (NumberOfProjectiles == 1)
	{
		return FString::Printf(TEXT(
			"<Title>%s: LEVEL %i</>\n\n <Default>Launches %i bolt of %s exploding on impact and dealing:</> <Damage>%i</><Default> %s damage with a chance to burn </>"),
			*ProjectileName, Level, NumberOfProjectiles, *ElementType, Damage, *ElementType);
	}
	
	return FString::Printf(TEXT(
		"<Title>%s: LEVEL %i</>\n\n <Default>Launches %i bolts of %s exploding on impact and dealing:</> <Damage>%i</><Default> %s damage with a chance to burn </>"),
		*ProjectileName, Level, NumberOfProjectiles, *ElementType, Damage, *ElementType);
}

FString UAuraProjectileSpell::GetNextLevelDescription(int32 Level)
{
	const FString ProjectileBluePrint = ProjectileClass->GetName();
	FString ProjectileName = "";
	FString ElementType = "";
	const int32 NumberOfProjectiles = Level <=5 ? Level : 5;
	const int32 Damage = DamageTypes[FAuraGameplayTags::Get().Damage_Fire].GetValueAtLevel(Level);

	if (ProjectileBluePrint == "BP_FireBolt_C")
	{
		ProjectileName = "Fire Bolt";
		ElementType = "fire";
	}


	if (NumberOfProjectiles == 1)
	{
		return FString::Printf(
			TEXT(
				"<Title>UPGRADE TO %s: LEVEL %i</>\n\n <Default>Launches %i bolt of %s exploding on impact and dealing:</> <Damage>%i</><Default> %s damage with a chance to burn </>"),
			*ProjectileName, Level, NumberOfProjectiles, *ElementType, Damage, *ElementType);
	}

	return FString::Printf(
		TEXT(
			"<Title>UPGRADE TO %s: LEVEL %i</>\n\n <Default>Launches %i bolts of %s exploding on impact and dealing:</> <Damage>%i</><Default> %s damage with a chance to burn </>"),
		*ProjectileName, Level, NumberOfProjectiles, *ElementType, Damage, *ElementType);
}
