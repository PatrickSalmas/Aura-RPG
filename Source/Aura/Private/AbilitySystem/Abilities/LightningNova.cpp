// Copyright Salmas Corporation


#include "AbilitySystem/Abilities/LightningNova.h"

void ULightningNova::SpawnLightningNovaWave()
{
	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	if (!IsValid(AvatarActor)) return;

	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
	if (!SourceASC) return;

	const FVector Origin = AvatarActor->GetActorLocation();

	AExpandingAOE* Wave = GetWorld()->SpawnActorDeferred<AExpandingAOE>(
		ExpandingAOEClass,
		FTransform(Origin),
		AvatarActor,
		Cast<APawn>(AvatarActor),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn
	);

	if (!Wave)
	{
		UE_LOG(LogTemp, Warning, TEXT("AOE Wave null"));
		return;
	}

	Wave->Shape = EAOEShape::FullCircle;
	Wave->Origin = Origin;
	Wave->MaxRadius = 1200.f;
	Wave->WaveSpeed = 2400.f;

	// Start with filled expansion.
	// Later we can test WaveThickness.
	Wave->WaveThickness = 0.f;

	Wave->SourceASC = SourceASC;
	Wave->DamageEffectClass = DamageEffectClass;
	Wave->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
	// Wave->DebuffEffectClass = StunEffectClass;

	Wave->FinishSpawning(FTransform(Origin));
}
