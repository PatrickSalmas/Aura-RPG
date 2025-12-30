// Copyright Salmas Corporation

#pragma once

#include "CoreMinimal.h"
#include "AuraAbilityTypes.h"
#include "GameplayEffectTypes.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "AuraProjectile.generated.h"

class UProjectileMovementComponent;
class UNiagaraSystem;

UCLASS()
class AURA_API AAuraProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAuraProjectile();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)	
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn = true))
	FDamageEffectParams DamageEffectParams;

	UPROPERTY()
	TObjectPtr<USceneComponent> HomingTargetSceneComponent;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool bHit = false;
	
	UFUNCTION(BlueprintCallable)
	virtual void OnHit();
	
	virtual void Destroyed() override;

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                     int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	bool IsValidOverlap(AActor* OtherActor);

	UPROPERTY()
	TObjectPtr<UAudioComponent> LoopingSoundComponent;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<USphereComponent> Sphere;
	
private:

	UPROPERTY(EditDefaultsOnly)
	float LifeSpan = 15.f;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ImpactSound;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> LoopingSound;
};
