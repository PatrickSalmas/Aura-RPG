#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GroundAOEFunctionLibrary.generated.h"

USTRUCT(BlueprintType)
struct FGroundAOEPlacementResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	bool bValid = false;

	UPROPERTY(BlueprintReadOnly)
	FVector Location = FVector::ZeroVector;

	UPROPERTY(BlueprintReadOnly)
	FVector Normal = FVector::UpVector;

	UPROPERTY(BlueprintReadOnly)
	float SlopeDegrees = 0.0f;

	UPROPERTY(BlueprintReadOnly)
	AActor* HitActor = nullptr;

	UPROPERTY(BlueprintReadOnly)
	UPrimitiveComponent* HitComponent = nullptr;
};

UCLASS()
class AURA_API UGroundAOEFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category="Ground AOE", meta=(WorldContext="WorldContextObject"))
	static FGroundAOEPlacementResult ValidateGroundAOEPlacement(
		UObject* WorldContextObject,
		FVector TestLocation,
		float MaxAllowedSlopeDegrees = 35.0f,
		float TraceUpDistance = 200.0f,
		float TraceDownDistance = 1000.0f,
		float GroundOffset = 2.0f,
		TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Visibility,
		bool bDrawDebug = false
	);

	UFUNCTION(BlueprintPure, Category="Ground AOE")
	static float GetSlopeDegreesFromNormal(FVector SurfaceNormal);
};