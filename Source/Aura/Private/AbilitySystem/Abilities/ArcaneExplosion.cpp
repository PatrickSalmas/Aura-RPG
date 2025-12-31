// Copyright Salmas Corporation


#include "AbilitySystem/Abilities/ArcaneExplosion.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Actor/ArcaneSplinter.h"

FString UArcaneExplosion::GetDescription(int32 Level)
{
	return Super::GetDescription(Level);
}

FString UArcaneExplosion::GetNextLevelDescription(int32 Level)
{
	return Super::GetNextLevelDescription(Level);
}

TArray<AArcaneSplinter*> UArcaneExplosion::SpawnArcaneSplinters()
{
	TArray<AArcaneSplinter*> Fireballs;
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	TArray<FRotator> Rotators = UAuraAbilitySystemLibrary::EvenlySpaceRotators(Forward, FVector::UpVector, 360.f, NumSplinters);

	for (const FRotator& Rotator : Rotators)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(Location);
		SpawnTransform.SetRotation(Rotator.Quaternion());
		AArcaneSplinter* ArcaneSplinter = GetWorld()->SpawnActorDeferred<AArcaneSplinter>(
			ArcaneSplinterClass, SpawnTransform, GetOwningActorFromActorInfo(), CurrentActorInfo->PlayerController->GetPawn(),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		// FireBall->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		// FireBall->ReturnToActor = GetAvatarActorFromActorInfo();
		//
		// FireBall->ExplosionDamageParams = MakeDamageEffectParamsFromClassDefaults();
		// FireBall->SetOwner(GetAvatarActorFromActorInfo());
		// if (GetAbilityLevel() > 1)
		// {
		// 	FireBall->IsRotatingFireBall = true;
		// }
		//
		//
		// Fireballs.Add(FireBall);
		//
		// FireBall->FinishSpawning(SpawnTransform);
	}
	
	return Fireballs;
}
