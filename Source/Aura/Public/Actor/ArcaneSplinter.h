// Copyright Salmas Corporation

#pragma once

#include "CoreMinimal.h"
#include "Actor/AuraProjectile.h"
#include "ArcaneSplinter.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AArcaneSplinter : public AAuraProjectile
{
	GENERATED_BODY()


protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
							 const FHitResult& SweepResult) override;

	virtual void OnHit() override;
};
