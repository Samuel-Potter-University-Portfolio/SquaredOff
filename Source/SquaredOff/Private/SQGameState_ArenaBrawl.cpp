// Fill out your copyright notice in the Description page of Project Settings.

#include "SquaredOff.h"
#include "SQGameState_ArenaBrawl.h"


FText ASQGameState_ArenaBrawl::GetFormattedPhase() 
{
	return FText::FromString(TEXT("Phase 69"));
}

FText ASQGameState_ArenaBrawl::GetFormattedDisplayedTime()
{
	int time = (int)GetDisplayedTime();
	int minutes = time / 60;
	int seconds = time % 60;

	if(seconds < 10)
		return FText::FromString(FString::Printf(TEXT("%i:0%i"), minutes, seconds));
	else
		return FText::FromString(FString::Printf(TEXT("%i:%i"), minutes, seconds));
}