// SquaredOff - TODO: Write notice

#include "SquaredOff.h"
#include "SQKnockablePawn.h"

ASQKnockablePawn::ASQKnockablePawn()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ASQKnockablePawn::Tick(float DeltaSeconds) 
{
	Super::Tick(DeltaSeconds);

	if (stun_timer > 0)
		stun_timer -= DeltaSeconds;
}

bool ASQKnockablePawn::AttemptKnock_Implementation(FVector force)
{
	if (!affected_movement || force.SizeSquared() < minimum_hit_magnitude * minimum_hit_magnitude)
		return false;

	//Increment hit factor
	float force_size = force.Size();
	force *= hit_factor;

	Stun(hit_factor * stun_scale);
	affected_movement->AddExternalForce(force);
	OnKnock(force);

	hit_factor += force_size * hit_increase_factor / minimum_hit_magnitude;
	return true;
}

void ASQKnockablePawn::OnKnock_Implementation(FVector force)
{

}

void ASQKnockablePawn::Stun(float time) 
{
	if (stun_timer < time) 
	{
		stun_timer = time; 
		OnStun();
	}
};

void ASQKnockablePawn::OnStun_Implementation()
{

}