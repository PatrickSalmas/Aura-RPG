// Copyright Salmas Coporation


#include "Player/AuraPlayerState.h"

AAuraPlayerState::AAuraPlayerState()
{
	// NetUpdateFrequency is deprecated so we are instead calling SetNetUpdateFrequency
	// NetUpdateFrequency = 100.f;
	SetNetUpdateFrequency(100.f);
}
