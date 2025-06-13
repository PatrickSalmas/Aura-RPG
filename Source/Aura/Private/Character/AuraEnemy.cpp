// Copyright Salmas Coporation


#include "Character/AuraEnemy.h"

void AAuraEnemy::HightlightActor()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "HightlightActor");
	bHighlighted = true;
}

void AAuraEnemy::UnHightlightActor()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, "UnHightlightActor");
	bHighlighted = false;
}
