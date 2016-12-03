// Fill out your copyright notice in the Description page of Project Settings.

#include "SquaredOff.h"
#include "SQMovementComponent.h"


void USQMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!affected_body)
		affected_body = (UPrimitiveComponent*) UpdatedComponent;

	if (!PawnOwner || !UpdatedComponent || !affected_body || GetOwnerRole() < ROLE_AutonomousProxy)
		return;

	//Check if on ground
	{
		FHitResult hit;
		FCollisionQueryParams params;
		params.AddIgnoredActor(PawnOwner);
		FVector location = PawnOwner->GetActorLocation();

		if (jump_cooldown > 0)
			jump_cooldown -= DeltaTime;

		on_ground = GetWorld()->LineTraceSingleByChannel(
			hit,
			location,
			location + FVector(0, 0, -100),
			ECC_WorldStatic,
			params
			);

		if (on_ground)
			jump_count = 0;
		else if (jump_count == 0)
			jump_count = 1;
	}

	//Handle movement
	{
		FVector frame_impulse = ConsumeInputVector().GetClampedToMaxSize(1.0f)  * move_speed * DeltaTime;
		HandleMovement(
			can_move ? frame_impulse + additional_force : additional_force,
			jumped_this_frame
			);

		jumped_this_frame = false;
		additional_force = FVector::ZeroVector;
	}
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

bool USQMovementComponent::Jump() 
{
	if (CanJump())
	{
		jump_count++;
		jump_cooldown = 0.5;
		on_ground = false;
		jumped_this_frame = true;
		return true;
	}
	else
		return false;
}