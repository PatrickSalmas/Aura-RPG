// Copyright Salmas Corporation


#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "LandscapeHeightfieldCollisionComponent.h"
#include "NavigationSystem.h"
#include "Character/AuraCharacter.h"
#include "Character/AuraEnemy.h"
#include "Components/CapsuleComponent.h"

class AAuraEnemy;
class UNavigationSystemV1;

void UAuraDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);
	const float ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, DamageType, ScaledDamage);
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(
		*DamageSpecHandle.Data.Get(), UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}

FDamageEffectParams UAuraDamageGameplayAbility::MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor,
	FVector InRadialDamageOrigin, bool bOverrideKnockbackDirection, FVector KnockBackDirectionOverride,
	bool bOverrideDeathImpulse, FVector DeathImpulseDirectionOverride, bool bOverridePitch, float PitchOverride) const
{
	FDamageEffectParams DamageEffectParams;
	DamageEffectParams.WorldContextObject = GetAvatarActorFromActorInfo();
	DamageEffectParams.DamageGameplayEffectClass = DamageEffectClass;
	DamageEffectParams.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	DamageEffectParams.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	DamageEffectParams.BaseDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	DamageEffectParams.AbilityLevel = GetAbilityLevel();
	DamageEffectParams.DamageType = DamageType;
	DamageEffectParams.DebuffChance = DebuffChance;
	DamageEffectParams.DebuffDamage = DebuffDamage;
	DamageEffectParams.DebuffDuration = DebuffDuration;
	DamageEffectParams.DebuffFrequency = DebuffFrequency;
	DamageEffectParams.ReactiveStatusChance = ReactiveStatusChance;
	DamageEffectParams.bCanApplyReactionStatus = bCanApplyReactionStatus;
	DamageEffectParams.bCanTriggerReaction = bCanTriggerReaction;
	DamageEffectParams.DeathImpulseMagnitude = DeathImpulseMagnitude;
	DamageEffectParams.KnockBackChance = KnockBackChance;
	DamageEffectParams.KnockBackImpulseMagnitude = KnockBackImpulseMagnitude;

	if (IsValid(TargetActor))
	{
		FRotator Rotation = (TargetActor->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).Rotation();
		if (bOverridePitch)
		{
			Rotation.Pitch = PitchOverride;
		}
		const FVector ToTarget = Rotation.Vector();
		if (!bOverrideKnockbackDirection)
		{
			DamageEffectParams.KnockBackImpulse = ToTarget * KnockBackImpulseMagnitude;
		}
		if (!bOverrideDeathImpulse)
		{
			DamageEffectParams.KnockBackImpulse = ToTarget * KnockBackImpulseMagnitude;
		}
	}
	
	if (bOverrideKnockbackDirection)
	{
		KnockBackDirectionOverride.Normalize();
		DamageEffectParams.KnockBackImpulse = KnockBackDirectionOverride * KnockBackImpulseMagnitude;
		if (bOverridePitch)
		{
			FRotator KnockbackRotation = KnockBackDirectionOverride.Rotation();
			KnockbackRotation.Pitch = PitchOverride;
			DamageEffectParams.KnockBackImpulse = KnockbackRotation.Vector() * KnockBackImpulseMagnitude;
		}
	}
	
	if (bOverrideDeathImpulse)
	{
		DeathImpulseDirectionOverride.Normalize();
		DamageEffectParams.DeathImpulse = DeathImpulseDirectionOverride * DeathImpulseMagnitude;
		if (bOverridePitch)
		{
			FRotator DeathImpulseRotation = DeathImpulseDirectionOverride.Rotation();
			DeathImpulseRotation.Pitch = PitchOverride;
			DamageEffectParams.DeathImpulse = DeathImpulseRotation.Vector() * DeathImpulseMagnitude;
		}
	}

	if (bIsRadialDamage)
	{
		DamageEffectParams.bIsRadialDamage = bIsRadialDamage;
		DamageEffectParams.RadialDamageOrigin = InRadialDamageOrigin;
		DamageEffectParams.RadialDamageInnerRadius = RadialDamageInnerRadius;
		DamageEffectParams.RadialDamageOuterRadius = RadialDamageOuterRadius;
	}
	return DamageEffectParams;
}

float UAuraDamageGameplayAbility::GetDamageAtLevel() const
{
	return Damage.GetValueAtLevel(GetAbilityLevel());
}

bool UAuraDamageGameplayAbility::IsValidTeleportGround(const FHitResult& Hit)
{
	const UPrimitiveComponent* HitComponent = Hit.GetComponent();

	if (!HitComponent)
	{
		return false;
	}

	// Landscapes are valid ground.
	if (HitComponent->IsA<ULandscapeHeightfieldCollisionComponent>())
	{
		return true;
	}

	static const FName TeleportGroundTag = FName("TeleportGround");

	if (HitComponent->ComponentHasTag(TeleportGroundTag))
	{
		return true;
	}

	if (const AActor* HitActor = Hit.GetActor())
	{
		if (HitActor->ActorHasTag(TeleportGroundTag))
		{
			return true;
		}
	}

	return false;
}

bool UAuraDamageGameplayAbility::GetGroundLocationFromTarget(
	const FVector& TargetLocation,
	FVector& OutGroundLocation,
	AActor* ActorToIgnore
) const
{
	if (!GetWorld())
	{
		OutGroundLocation = TargetLocation;
		return false;
	}

	const FVector TraceStart = TargetLocation + FVector(0.f, 0.f, 2000.f);
	const FVector TraceEnd   = TargetLocation + FVector(0.f, 0.f, -5000.f);

	TArray<FHitResult> HitResults;

	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = false;
	QueryParams.AddIgnoredActor(GetAvatarActorFromActorInfo());

	if (ActorToIgnore)
	{
		QueryParams.AddIgnoredActor(ActorToIgnore);
	}

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);

	const bool bHit = GetWorld()->LineTraceMultiByObjectType(
		HitResults,
		TraceStart,
		TraceEnd,
		ObjectQueryParams,
		QueryParams
	);

	if (!bHit)
	{
		OutGroundLocation = TargetLocation;
		return false;
	}

	for (const FHitResult& Hit : HitResults)
	{
		if (IsValidTeleportGround(Hit))
		{
			OutGroundLocation = Hit.ImpactPoint;
			return true;
		}
	}

	OutGroundLocation = TargetLocation;
	return false;
}

bool UAuraDamageGameplayAbility::GetNearestValidGroundLocationFromTarget(
	const FVector& MouseHitLocation,
	AActor* MouseHitActor,
	FVector& OutGroundLocation,
	FVector& OutTeleportLocation,
	float ExtraDistanceFromTarget
) const
{
	AActor* AvatarActor = GetAvatarActorFromActorInfo();

	if (!GetWorld() || !AvatarActor)
	{
		OutGroundLocation = MouseHitLocation;
		OutTeleportLocation = MouseHitLocation;
		return false;
	}

	// Helper lambda: converts a ground point into a safe character teleport point.
	auto BuildTeleportLocationFromGroundLocation = [AvatarActor](const FVector& GroundLocation)
	{
		FVector TeleportLocation = GroundLocation;

		if (const AAuraCharacter* AuraCharacter = Cast<AAuraCharacter>(AvatarActor))
		{
			if (const UCapsuleComponent* Capsule = AuraCharacter->GetCapsuleComponent())
			{
				TeleportLocation.Z += Capsule->GetScaledCapsuleHalfHeight();
			}
		}

		return TeleportLocation;
	};

	AAuraEnemy* EnemyTarget = Cast<AAuraEnemy>(MouseHitActor);

	// ---------------------------------------------------------------------
	// CASE 1: Mouse hit normal ground / wall / anything that is NOT an enemy.
	// Return the cursor's ground location directly.
	// Do NOT project to NavMesh here, because that can shift the point.
	// ---------------------------------------------------------------------
	if (!EnemyTarget)
	{
		FVector GroundLocation;

		const bool bFoundGround = GetGroundLocationFromTarget(
			MouseHitLocation,
			GroundLocation,
			nullptr
		);

		if (bFoundGround)
		{
			OutGroundLocation = GroundLocation;
			OutTeleportLocation = BuildTeleportLocationFromGroundLocation(GroundLocation);
			return true;
		}

		OutGroundLocation = MouseHitLocation;
		OutTeleportLocation = BuildTeleportLocationFromGroundLocation(MouseHitLocation);
		return false;
	}

	// ---------------------------------------------------------------------
	// CASE 2: Mouse hit an enemy.
	// Find a nearby valid ground location beside the enemy.
	// ---------------------------------------------------------------------

	const FVector EnemyLocation = EnemyTarget->GetActorLocation();

	FVector DirectionFromEnemyToCaster = AvatarActor->GetActorLocation() - EnemyLocation;
	DirectionFromEnemyToCaster.Z = 0.f;

	if (DirectionFromEnemyToCaster.IsNearlyZero())
	{
		DirectionFromEnemyToCaster = FVector::ForwardVector;
	}

	DirectionFromEnemyToCaster.Normalize();

	// Estimate enemy radius.
	FVector EnemyOrigin;
	FVector EnemyExtent;
	EnemyTarget->GetActorBounds(false, EnemyOrigin, EnemyExtent);

	const float EnemyRadius2D = FMath::Max(EnemyExtent.X, EnemyExtent.Y);

	// Estimate caster/player radius.
	float CasterRadius = 50.f;

	if (const AAuraCharacter* AuraCharacter = Cast<AAuraCharacter>(AvatarActor))
	{
		if (const UCapsuleComponent* Capsule = AuraCharacter->GetCapsuleComponent())
		{
			CasterRadius = Capsule->GetScaledCapsuleRadius();
		}
	}

	const float SafeDistance =
		EnemyRadius2D +
		CasterRadius +
		ExtraDistanceFromTarget;

	const FVector DesiredLocation =
		EnemyLocation + DirectionFromEnemyToCaster * SafeDistance;

	FVector InitialGroundLocation;

	const bool bFoundInitialGround = GetGroundLocationFromTarget(
		DesiredLocation,
		InitialGroundLocation,
		EnemyTarget
	);

	if (!bFoundInitialGround)
	{
		OutGroundLocation = DesiredLocation;
		OutTeleportLocation = BuildTeleportLocationFromGroundLocation(DesiredLocation);
		return false;
	}

	// ---------------------------------------------------------------------
	// Optional NavMesh projection for enemy-targeted relocation.
	// Important: only use the NavMesh projected X/Y.
	// Do NOT trust the NavMesh projected Z for teleport placement.
	// ---------------------------------------------------------------------
	if (UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld()))
	{
		FNavLocation ProjectedLocation;

		const FVector ProjectionExtent = FVector(150.f, 150.f, 300.f);

		const bool bProjected = NavSystem->ProjectPointToNavigation(
			InitialGroundLocation,
			ProjectedLocation,
			ProjectionExtent
		);

		if (bProjected)
		{
			const FVector ProjectedXYLocation(
				ProjectedLocation.Location.X,
				ProjectedLocation.Location.Y,
				InitialGroundLocation.Z
			);

			FVector FinalGroundLocation;

			const bool bFoundFinalGround = GetGroundLocationFromTarget(
				ProjectedXYLocation,
				FinalGroundLocation,
				EnemyTarget
			);

			if (bFoundFinalGround)
			{
				OutGroundLocation = FinalGroundLocation;
				OutTeleportLocation = BuildTeleportLocationFromGroundLocation(FinalGroundLocation);
				return true;
			}

			// Fallback: use projected X/Y, but preserve the original traced ground Z.
			OutGroundLocation = ProjectedXYLocation;
			OutTeleportLocation = BuildTeleportLocationFromGroundLocation(ProjectedXYLocation);
			return true;
		}
	}

	// Fallback if NavMesh projection fails.
	OutGroundLocation = InitialGroundLocation;
	OutTeleportLocation = BuildTeleportLocationFromGroundLocation(InitialGroundLocation);
	return true;
}

FTaggedMontage UAuraDamageGameplayAbility::GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages)
{
	if (TaggedMontages.Num() > 0)
	{
		const int32 Selection = FMath::RandRange(0, TaggedMontages.Num() - 1);
		return TaggedMontages[Selection];
	}

	return FTaggedMontage();
}
