// Fill out your copyright notice in the Description page of Project Settings.

#include "SquaredOff.h"
#include "SQMovementComponent.h"


void USQMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!affected_body)
		affected_body = (UPrimitiveComponent*) UpdatedComponent;

	if (!PawnOwner || !UpdatedComponent || !affected_body)
		return;


	FVector frame_impulse = ConsumeInputVector().GetClampedToMaxSize(1.0f)  * move_speed * DeltaTime;
	HandleMovement(
		can_move ? frame_impulse + additional_force : additional_force,
		jumped_this_frame
		);

	jumped_this_frame = false;
	additional_force = FVector::ZeroVector;
}

void USQMovementComponent::HandleMovement_Implementation(const FVector movement_input, const bool jumped)
{
	affected_body->AddImpulse(movement_input);

	if (jumped)
		affected_body->AddImpulse(FVector(0, 0, jump_force));
}
bool USQMovementComponent::HandleMovement_Validate(const FVector movement_input, const bool jumped)
{
	return true;
}