// Copyright Salmas Corporation


#include "AbilitySystem/Abilities/LightningNova.h"

void ULightningNova::SpawnLightningNovaWave(FVector OriginOverride, AActor* ActorOverride)
{
	AActor* AvatarActor = ActorOverride == nullptr ? GetAvatarActorFromActorInfo() : ActorOverride;
	if (!IsValid(AvatarActor)) return;

	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();
	if (!SourceASC) return;
	
	const FVector Origin = !OriginOverride.IsZero() ? OriginOverride : AvatarActor->GetActorLocation();

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
	Wave->MaxRadius = 700.f;
	Wave->WaveSpeed = 2200.f;

	// Start with filled expansion.
	// Later we can test WaveThickness.
	Wave->WaveThickness = 0.f;

	Wave->SourceASC = SourceASC;
	Wave->DamageEffectClass = DamageEffectClass;
	
	Wave->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();

	Wave->FinishSpawning(FTransform(Origin));
}
