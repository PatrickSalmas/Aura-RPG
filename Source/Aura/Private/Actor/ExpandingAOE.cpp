// Copyright Salmas Corporation


#include "Actor/ExpandingAOE.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Character/AuraCharacterBase.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AExpandingAOE::AExpandingAOE()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("NiagaraComponent");
	SetRootComponent(NiagaraComponent);
}

// Called when the game starts or when spawned
void AExpandingAOE::BeginPlay()
{
	Super::BeginPlay();

	if (Origin.IsNearlyZero())
	{
		Origin = GetActorLocation();
	}

	WaveForward.Z = 0.f;
	WaveForward.Normalize();
	
	if (NiagaraComponent && NiagaraSystem)
	{
		if (Shape == EAOEShape::Cone)
		{
			NiagaraComponent->SetWorldRotation(WaveForward.Rotation());
		}
		NiagaraComponent->SetAsset(NiagaraSystem);
		NiagaraComponent->Activate(true);

		// NiagaraComponent->SetFloatParameter(MaxRadiusParameterName, MaxRadius);
		// NiagaraComponent->SetFloatParameter(ConeAngleParameterName, ConeAngleDegrees);
		// NiagaraComponent->SetFloatParameter(WaveThicknessParameterName, WaveThickness);
	}
}

void AExpandingAOE::UpdateWave(float DeltaTime)
{
	ElapsedTime += DeltaTime;

	PreviousRadius = CurrentRadius;
	CurrentRadius = FMath::Min(ElapsedTime * WaveSpeed, MaxRadius);
}

void AExpandingAOE::CheckForHits()
{
	TArray<AActor*> OverlappingActors;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());

	UKismetSystemLibrary::SphereOverlapActors(
		this,
		Origin,
		CurrentRadius,
		ObjectTypes,
		nullptr,
		ActorsToIgnore,
		OverlappingActors
	);

	for (AActor* Target : OverlappingActors)
	{
		if (!IsValid(Target)) continue;
		if (HitActors.Contains(Target)) continue;

		if (!IsTargetInsideWave(Target)) continue;

		HitActors.Add(Target);
		ApplyHit(Target);
	}
}

bool AExpandingAOE::IsTargetInsideWave(AActor* Target) const
{
	if (!IsValid(Target)) return false;

	FVector ToTarget = Target->GetActorLocation() - Origin;
	ToTarget.Z = 0.f;

	const float Distance = ToTarget.Size();

	if (Distance > CurrentRadius)
	{
		return false;
	}

	// Optional wavefront thickness.
	// If WaveThickness is 0, this behaves like a filled expanding area.
	if (WaveThickness > 0.f)
	{
		const float InnerRadius = FMath::Max(0.f, CurrentRadius - WaveThickness);

		if (Distance < InnerRadius)
		{
			return false;
		}
	}

	if (Shape == EAOEShape::FullCircle)
	{
		return true;
	}

	if (Shape == EAOEShape::Cone)
	{
		if (Distance <= KINDA_SMALL_NUMBER)
		{
			return true;
		}

		FVector DirectionToTarget = ToTarget / Distance;

		FVector Forward = WaveForward;
		Forward.Z = 0.f;
		Forward.Normalize();

		const float HalfAngleDegrees = ConeAngleDegrees * 0.5f;
		const float MinDot = FMath::Cos(FMath::DegreesToRadians(HalfAngleDegrees));

		const float Dot = FVector::DotProduct(Forward, DirectionToTarget);

		return Dot >= MinDot;
	}

	return false;
}

void AExpandingAOE::ApplyHit(AActor* Target)
{
	if (!IsValid(Target)) return;
	if (!SourceASC) return;

	UAbilitySystemComponent* TargetASC =
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);

	if (!TargetASC) return;

	// Temporary debug.
	UE_LOG(LogTemp, Warning, TEXT("Expanding AOE hit: %s"), *Target->GetName());

	
	if (HasAuthority())
	{
		if (TargetASC)
		{
			const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
			DamageEffectParams.DeathImpulse = DeathImpulse;
			
			// FRotator Rotation = GetActorRotation();
			// Rotation.Pitch = 45.f;
			// const  FVector KnockbackDirection = Rotation.Vector();
			// DamageEffectParams.KnockBackImpulse = KnockbackDirection * DamageEffectParams.KnockBackImpulseMagnitude;
			
			DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
			UAuraAbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
			
			if (DoesCancelActions)
			{
				// Target
				AAuraCharacterBase* CharacterBase = Cast<AAuraCharacterBase>(Target);
				CharacterBase->CancelActions();
			}
		}
	}
}

// Called every frame
void AExpandingAOE::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateWave(DeltaTime);
	CheckForHits();

	if (CurrentRadius >= MaxRadius)
	{
		Destroy();
	}
}

