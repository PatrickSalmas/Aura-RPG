#include "AbilitySystem/Abilities/GroundAOEFunctionLibrary.h"

#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"

float UGroundAOEFunctionLibrary::GetSlopeDegreesFromNormal(FVector SurfaceNormal)
{
	if (SurfaceNormal.IsNearlyZero())
	{
		return 90.0f;
	}

	SurfaceNormal.Normalize();

	const float DotWithUp = FVector::DotProduct(SurfaceNormal, FVector::UpVector);
	const float ClampedDot = FMath::Clamp(DotWithUp, -1.0f, 1.0f);

	return FMath::RadiansToDegrees(FMath::Acos(ClampedDot));
}

FGroundAOEPlacementResult UGroundAOEFunctionLibrary::ValidateGroundAOEPlacement(
	UObject* WorldContextObject,
	FVector TestLocation,
	float MaxAllowedSlopeDegrees,
	float TraceUpDistance,
	float TraceDownDistance,
	float GroundOffset,
	TEnumAsByte<ECollisionChannel> TraceChannel,
	bool bDrawDebug
)
{
	FGroundAOEPlacementResult Result;

	if (!WorldContextObject)
	{
		return Result;
	}

	UWorld* World = WorldContextObject->GetWorld();
	if (!World)
	{
		return Result;
	}

	const FVector TraceStart = TestLocation + FVector(0.0f, 0.0f, TraceUpDistance);
	const FVector TraceEnd = TestLocation - FVector(0.0f, 0.0f, TraceDownDistance);

	FHitResult Hit;
	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = false;

	const bool bHit = World->LineTraceSingleByChannel(
		Hit,
		TraceStart,
		TraceEnd,
		TraceChannel,
		QueryParams
	);

	if (bDrawDebug)
	{
		const FColor DebugColor = bHit ? FColor::Green : FColor::Red;
		DrawDebugLine(World, TraceStart, TraceEnd, DebugColor, false, 2.0f, 0, 2.0f);

		if (bHit)
		{
			DrawDebugSphere(World, Hit.ImpactPoint, 16.0f, 12, FColor::Yellow, false, 2.0f);
			DrawDebugDirectionalArrow(
				World,
				Hit.ImpactPoint,
				Hit.ImpactPoint + Hit.ImpactNormal * 120.0f,
				30.0f,
				FColor::Cyan,
				false,
				2.0f,
				0,
				3.0f
			);
		}
	}

	if (!bHit)
	{
		return Result;
	}

	const float SlopeDegrees = GetSlopeDegreesFromNormal(Hit.ImpactNormal);

	Result.SlopeDegrees = SlopeDegrees;
	Result.Normal = Hit.ImpactNormal;
	Result.Location = Hit.ImpactPoint + Hit.ImpactNormal * GroundOffset;
	Result.HitActor = Hit.GetActor();
	Result.HitComponent = Hit.GetComponent();

	if (SlopeDegrees > MaxAllowedSlopeDegrees)
	{
		Result.bValid = false;
		return Result;
	}

	Result.bValid = true;
	return Result;
}