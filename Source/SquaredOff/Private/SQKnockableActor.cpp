// SquaredOff - TODO: Write notice

#include "SquaredOff.h"
#include "SQKnockableActor.h"

ASQKnockableActor::ASQKnockableActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

bool ASQKnockableActor::AttemptKnock_Implementation(FVector force)
{
	if (!affected_body || force.SizeSquared() < minimum_hit_magnitude * minimum_hit_magnitude)
		return false;

	//Increment hit factor
	float force_size = force.Size();
	force *= hit_factor;

	AddForce(force);
	OnKnock(force);

	return true;
}

void ASQKnockableActor::OnKnock_Implementation(FVector force)
{
}

void ASQKnockableActor::AddForce_Implementation(const FVector force)
{
	if (affected_body)
		affected_body->AddImpulse(force);
	else
		UE_LOG(LogTemp, Error, TEXT("AB IS NULL"));
	UE_LOG(LogTemp, Error, TEXT("BOOP"));
}

bool ASQKnockableActor::AddForce_Validate(const FVector force)
{
	return true;
}