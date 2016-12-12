// Fill out your copyright notice in the Description page of Project Settings.

#include "SquaredOff.h"
#include "Net/UnrealNetwork.h"
#include "SQPlayerState.h"


void ASQPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASQPlayerState, is_ready);
	DOREPLIFETIME(ASQPlayerState, team_index);
}

void ASQPlayerState::SetReady(const bool ready_state)
{
	is_ready = ready_state;
	SetReadyState(ready_state);
}

void ASQPlayerState::SetReadyState_Implementation(const bool ready_state)
{
	is_ready = ready_state;
}

bool ASQPlayerState::SetReadyState_Validate(const bool ready_state)
{
	return true;
}