// Fill out your copyright notice in the Description page of Project Settings.

#include "SquaredOff.h"
#include "SQGameState_ArenaBrawl.h"
#include "Net/UnrealNetwork.h"

ASQGameState_ArenaBrawl::ASQGameState_ArenaBrawl() 
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASQGameState_ArenaBrawl::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASQGameState_ArenaBrawl, replicated_time);
	DOREPLIFETIME(ASQGameState_ArenaBrawl, is_match_over);
}

FText ASQGameState_ArenaBrawl::GetFormattedPhase() 
{
	return FText::FromString(TEXT("Phase 1"));
}

FText ASQGameState_ArenaBrawl::GetFormattedDisplayedTime()
{
	int time = (int)match_time;
	int minutes = time / 60;
	int seconds = time % 60;

	if(seconds < 10)
		return FText::FromString(FString::Printf(TEXT("%i:0%i"), minutes, seconds));
	else
		return FText::FromString(FString::Printf(TEXT("%i:%i"), minutes, seconds));
}

void ASQGameState_ArenaBrawl::Tick(float delta_seconds)
{
	Super::Tick(delta_seconds);
	match_time -= delta_seconds;

	if (match_time < 0.0f)
	{
		match_time = 0.0f;
	}
}