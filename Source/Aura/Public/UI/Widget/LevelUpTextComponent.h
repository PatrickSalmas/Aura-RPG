// Copyright Salmas Corporation
//OBSOLETE, NO LONGER USED. NEED TO SAFELY REMOVE FROM PROJECT

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "LevelUpTextComponent.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API ULevelUpTextComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetLevelUpText(int32 Level);
	
};
