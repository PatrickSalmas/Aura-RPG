// Copyright Salmas Corporation


#include "AbilitySystem/Abilities/FlameFlurry.h"

void UFlameFlurry::SpawnFlameWave(FDamageEffectParams DamageEffectParams)
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

	Wave->NiagaraSystem = FlameFlurryNiagaraSystem;
	
	Wave->Shape = EAOEShape::Cone;
	Wave->Origin = Origin;
	Wave->MaxRadius = 650.f;
	Wave->ConeAngleDegrees = 85.f;
	Wave->WaveSpeed = 2400.f;
	Wave->WaveForward = AvatarActor->GetActorForwardVector();
	Wave->DoesCancelActions = true;
	
	// Start with filled expansion.
	// Later we can test WaveThickness.
	Wave->WaveThickness = 0.f;

	Wave->SourceASC = SourceASC;
	Wave->DamageEffectClass = DamageEffectClass;
	
	Wave->DamageEffectParams = DamageEffectParams;

	Wave->FinishSpawning(FTransform(Origin));
}
