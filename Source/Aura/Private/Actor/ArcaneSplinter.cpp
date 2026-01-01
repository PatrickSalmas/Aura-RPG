// Copyright Salmas Corporation


#include "Actor/ArcaneSplinter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Character/AuraEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

void AArcaneSplinter::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValidOverlap(OtherActor) || SelectedTarget != OtherActor) return;
	if (!bHit) OnHit();

	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
			DamageEffectParams.DeathImpulse = DeathImpulse;
			
			FRotator Rotation = GetActorRotation();
			Rotation.Pitch = 45.f;
			const  FVector KnockbackDirection = Rotation.Vector();
			DamageEffectParams.KnockBackImpulse = KnockbackDirection * DamageEffectParams.KnockBackImpulseMagnitude;
			
			DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
			if (AAuraEnemy* EnemyTarget = Cast<AAuraEnemy>(OtherActor)) EnemyTarget->AddToAttachActors(this);
			UAuraAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
			Sphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
	else bHit = true;
}

void AArcaneSplinter::OnHit()
{
	// Super::OnHit();
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
	if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
	}
	bHit = true;
}

void AArcaneSplinter::Destroyed()
{
	Super::Destroyed();
	ImpactSound = nullptr;
}
