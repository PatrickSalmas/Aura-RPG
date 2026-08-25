#include "AuraAbilityTypes.h"

bool FAuraGameplayEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
    bOutSuccess = true;

    enum ERepBit : uint8
    {
        Rep_Instigator = 0,
        Rep_EffectCauser,
        Rep_AbilityCDO,
        Rep_SourceObject,
        Rep_Actors,
        Rep_HitResult,
        Rep_WorldOrigin,

        Rep_IsBlockedHit,
        Rep_IsCriticalHit,
        Rep_IsSuccessfulDebuff,
        Rep_DebuffDamage,
        Rep_DebuffDuration,
        Rep_DebuffFrequency,
        Rep_DamageType,
        Rep_DeathImpulse,
        Rep_KnockbackImpulse,

        Rep_IsRadialDamage,
        Rep_RadialDamageInnerRadius,
        Rep_RadialDamageOuterRadius,
        Rep_RadialDamageOrigin,

        Rep_IsSuccessfulReactiveStatus,
        Rep_ReactiveStatus,
        Rep_ReactiveStatusChance,
        Rep_TriggeredReaction,
        Rep_ReactiveStatusToConsume,
        Rep_CanApplyReactionStatus,
        Rep_CanTriggerReaction,
        Rep_IsSuccessfulKnockback,
        Rep_ShouldHitReact,

        Rep_NumBits
    };

    static_assert(
        Rep_NumBits <= 32,
        "FAuraGameplayEffectContext has exceeded the capacity of uint32 RepBits.");

    uint32 RepBits = 0;

    const auto SetRepBit = [&RepBits](const ERepBit Bit, const bool bValue)
    {
        if (bValue)
        {
            RepBits |= 1u << Bit;
        }
    };

    if (Ar.IsSaving())
    {
        SetRepBit(Rep_Instigator, bReplicateInstigator && Instigator.IsValid());
        SetRepBit(Rep_EffectCauser, bReplicateEffectCauser && EffectCauser.IsValid());
        SetRepBit(Rep_AbilityCDO, AbilityCDO.IsValid());
        SetRepBit(Rep_SourceObject, bReplicateSourceObject && SourceObject.IsValid());
        SetRepBit(Rep_Actors, !Actors.IsEmpty());
        SetRepBit(Rep_HitResult, HitResult.IsValid());
        SetRepBit(Rep_WorldOrigin, bHasWorldOrigin);

        SetRepBit(Rep_IsBlockedHit, bIsBlockedHit);
        SetRepBit(Rep_IsCriticalHit, bIsCriticalHit);
        SetRepBit(Rep_IsSuccessfulDebuff, bIsSuccessfulDebuff);
        SetRepBit(Rep_DebuffDamage, DebuffDamage != 0.f);
        SetRepBit(Rep_DebuffDuration, DebuffDuration != 0.f);
        SetRepBit(Rep_DebuffFrequency, DebuffFrequency != 0.f);
        SetRepBit(Rep_DamageType, DamageType.IsValid());
        SetRepBit(Rep_DeathImpulse, !DeathImpulse.IsZero());
        SetRepBit(Rep_KnockbackImpulse, !KnockBackImpulse.IsZero());

        SetRepBit(Rep_IsRadialDamage, bIsRadialDamage);
        SetRepBit(Rep_RadialDamageInnerRadius, bIsRadialDamage && RadialDamageInnerRadius != 0.f);
        SetRepBit(Rep_RadialDamageOuterRadius, bIsRadialDamage && RadialDamageOuterRadius != 0.f);
        SetRepBit(Rep_RadialDamageOrigin, bIsRadialDamage && !RadialDamageOrigin.IsZero());

        SetRepBit(Rep_IsSuccessfulReactiveStatus, bIsSuccessfulReactiveStatus);
        SetRepBit(Rep_ReactiveStatus, ReactiveStatus.IsValid());
        SetRepBit(Rep_ReactiveStatusChance, ReactiveStatusChance != 0.f);
        SetRepBit(Rep_TriggeredReaction, TriggeredReaction.IsValid());
        SetRepBit(Rep_ReactiveStatusToConsume, ReactiveStatusToConsume.IsValid());
        SetRepBit(Rep_CanApplyReactionStatus, bCanApplyReactionStatus);
        SetRepBit(Rep_CanTriggerReaction, bCanTriggerReaction);
        SetRepBit(Rep_IsSuccessfulKnockback, bIsSuccessfulKnockback);
        SetRepBit(Rep_ShouldHitReact, bShouldHitReact);
    }

    Ar.SerializeBits(&RepBits, Rep_NumBits);

    const auto HasRepBit = [RepBits](const ERepBit Bit)
    {
        return (RepBits & (1u << Bit)) != 0;
    };

    /*
     * Base FGameplayEffectContext data
     */

    if (HasRepBit(Rep_Instigator))
    {
        Ar << Instigator;
    }
    else if (Ar.IsLoading())
    {
        Instigator.Reset();
    }

    if (HasRepBit(Rep_EffectCauser))
    {
        Ar << EffectCauser;
    }
    else if (Ar.IsLoading())
    {
        EffectCauser.Reset();
    }

    if (HasRepBit(Rep_AbilityCDO))
    {
        Ar << AbilityCDO;
    }
    else if (Ar.IsLoading())
    {
        AbilityCDO.Reset();
    }

    if (HasRepBit(Rep_SourceObject))
    {
        Ar << SourceObject;
    }
    else if (Ar.IsLoading())
    {
        SourceObject.Reset();
    }

    if (HasRepBit(Rep_Actors))
    {
        SafeNetSerializeTArray_Default<31>(Ar, Actors);
    }
    else if (Ar.IsLoading())
    {
        Actors.Reset();
    }

    if (HasRepBit(Rep_HitResult))
    {
        if (Ar.IsLoading() && !HitResult.IsValid())
        {
            HitResult = MakeShared<FHitResult>();
        }

        bool bFieldSuccess = true;
        HitResult->NetSerialize(Ar, Map, bFieldSuccess);
        bOutSuccess &= bFieldSuccess;
    }
    else if (Ar.IsLoading())
    {
        HitResult.Reset();
    }

    if (HasRepBit(Rep_WorldOrigin))
    {
        Ar << WorldOrigin;
        bHasWorldOrigin = true;
    }
    else if (Ar.IsLoading())
    {
        WorldOrigin = FVector::ZeroVector;
        bHasWorldOrigin = false;
    }

    /*
     * Booleans encoded directly in RepBits
     */

    if (Ar.IsLoading())
    {
        bIsBlockedHit = HasRepBit(Rep_IsBlockedHit);
        bIsCriticalHit = HasRepBit(Rep_IsCriticalHit);
        bIsSuccessfulDebuff = HasRepBit(Rep_IsSuccessfulDebuff);
        bIsRadialDamage = HasRepBit(Rep_IsRadialDamage);
        bIsSuccessfulReactiveStatus = HasRepBit(Rep_IsSuccessfulReactiveStatus);
        bCanApplyReactionStatus = HasRepBit(Rep_CanApplyReactionStatus);
        bCanTriggerReaction = HasRepBit(Rep_CanTriggerReaction);
        bIsSuccessfulKnockback = HasRepBit(Rep_IsSuccessfulKnockback);
        bShouldHitReact = HasRepBit(Rep_ShouldHitReact);
    }

    /*
     * Debuff data
     */

    if (HasRepBit(Rep_DebuffDamage))
    {
        Ar << DebuffDamage;
    }
    else if (Ar.IsLoading())
    {
        DebuffDamage = 0.f;
    }

    if (HasRepBit(Rep_DebuffDuration))
    {
        Ar << DebuffDuration;
    }
    else if (Ar.IsLoading())
    {
        DebuffDuration = 0.f;
    }

    if (HasRepBit(Rep_DebuffFrequency))
    {
        Ar << DebuffFrequency;
    }
    else if (Ar.IsLoading())
    {
        DebuffFrequency = 0.f;
    }

    if (HasRepBit(Rep_DamageType))
    {
        if (Ar.IsLoading() && !DamageType.IsValid())
        {
            DamageType = MakeShared<FGameplayTag>();
        }

        bool bFieldSuccess = true;
        DamageType->NetSerialize(Ar, Map, bFieldSuccess);
        bOutSuccess &= bFieldSuccess;
    }
    else if (Ar.IsLoading())
    {
        DamageType.Reset();
    }

    /*
     * Impulses
     */

    if (HasRepBit(Rep_DeathImpulse))
    {
        bool bFieldSuccess = true;
        DeathImpulse.NetSerialize(Ar, Map, bFieldSuccess);
        bOutSuccess &= bFieldSuccess;
    }
    else if (Ar.IsLoading())
    {
        DeathImpulse = FVector::ZeroVector;
    }

    if (HasRepBit(Rep_KnockbackImpulse))
    {
        bool bFieldSuccess = true;
        KnockBackImpulse.NetSerialize(Ar, Map, bFieldSuccess);
        bOutSuccess &= bFieldSuccess;
    }
    else if (Ar.IsLoading())
    {
        KnockBackImpulse = FVector::ZeroVector;
    }

    /*
     * Radial-damage data
     */

    if (HasRepBit(Rep_RadialDamageInnerRadius))
    {
        Ar << RadialDamageInnerRadius;
    }
    else if (Ar.IsLoading())
    {
        RadialDamageInnerRadius = 0.f;
    }

    if (HasRepBit(Rep_RadialDamageOuterRadius))
    {
        Ar << RadialDamageOuterRadius;
    }
    else if (Ar.IsLoading())
    {
        RadialDamageOuterRadius = 0.f;
    }

    if (HasRepBit(Rep_RadialDamageOrigin))
    {
        bool bFieldSuccess = true;
        RadialDamageOrigin.NetSerialize(Ar, Map, bFieldSuccess);
        bOutSuccess &= bFieldSuccess;
    }
    else if (Ar.IsLoading())
    {
        RadialDamageOrigin = FVector::ZeroVector;
    }

    /*
     * Reactive-status and reaction data
     */

    if (HasRepBit(Rep_ReactiveStatus))
    {
        bool bFieldSuccess = true;
        ReactiveStatus.NetSerialize(Ar, Map, bFieldSuccess);
        bOutSuccess &= bFieldSuccess;
    }
    else if (Ar.IsLoading())
    {
        ReactiveStatus = FGameplayTag();
    }

    if (HasRepBit(Rep_ReactiveStatusChance))
    {
        Ar << ReactiveStatusChance;
    }
    else if (Ar.IsLoading())
    {
        ReactiveStatusChance = 0.f;
    }

    if (HasRepBit(Rep_TriggeredReaction))
    {
        bool bFieldSuccess = true;
        TriggeredReaction.NetSerialize(Ar, Map, bFieldSuccess);
        bOutSuccess &= bFieldSuccess;
    }
    else if (Ar.IsLoading())
    {
        TriggeredReaction = FGameplayTag();
    }

    if (HasRepBit(Rep_ReactiveStatusToConsume))
    {
        bool bFieldSuccess = true;
        ReactiveStatusToConsume.NetSerialize(Ar, Map, bFieldSuccess);
        bOutSuccess &= bFieldSuccess;
    }
    else if (Ar.IsLoading())
    {
        ReactiveStatusToConsume = FGameplayTag();
    }

    if (Ar.IsLoading())
    {
        AddInstigator(Instigator.Get(), EffectCauser.Get());
    }

    bOutSuccess = bOutSuccess && !Ar.IsError();

    return true;
}