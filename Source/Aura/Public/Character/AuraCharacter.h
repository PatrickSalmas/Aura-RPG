// Copyright Salmas Corporation

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "AuraCharacter.generated.h"

class ULevelUpTextComponent;
class UCameraComponent;
class USpringArmComponent;
class UNiagaraComponent;
/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()

public:
	AAuraCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	/* Player interface */
	virtual void AddToXP_Implementation(int32 InXP) override;
	virtual void LevelUp_Implementation() override;
	virtual int32 GetXP_Implementation() const override;
	virtual int32 FindLevelForXP_Implementation(int32 InXP) const override;
	virtual int32 GetAttributePointsReward_Implementation(int32 InPlayerLevel) const override;
	virtual int32 GetSpellPointsReward_Implementation(int32 InPlayerLevel) const override;
	virtual void AddToPlayerLevel_Implementation(int32 InPlayerLevel) override;
	virtual void AddToAttributesPoints_Implementation(int32 InAttributePoints) override;
	virtual void AddToSpellPoints_Implementation(int32 InSpellPoints) override;
	virtual int32 GetAttributePoints_Implementation() const override;
	virtual int32 GetSpellPoints_Implementation() const override;
	virtual void ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial) override;
	virtual void HideMagicCircle_Implementation() override;
	/* end Player interface */

	/* Combat interface */
	virtual int32 GetPlayerLevel_Implementation() override;
	/* end Combat interface */

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;

	virtual void OnRep_Stunned() override;

	virtual void OnRep_Burned() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USpringArmComponent> CameraBoom;
	
	/*
	 * Character animations and montages
	 */ 
	UPROPERTY(Category="Animations", EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* FireBoltMontage;
	
	UPROPERTY(Category="Animations", EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* FlameFlurryMontage;
	
	UPROPERTY(Category="Animations", EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* ArcaneShardsMontage;
	
	UPROPERTY(Category="Animations", EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* ElectrocuteMontage;

private:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> TopDownCameraComponent;
	
	virtual void InitAbilityActorInfo() override;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastLevelUpParticles() const;
};
