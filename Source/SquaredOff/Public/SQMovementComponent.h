// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PawnMovementComponent.h"
#include "SQMovementComponent.generated.h"


UCLASS()
class SQUAREDOFF_API USQMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category = "Movement")
	bool can_move = true;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Movement")
	int max_jumps = 2;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	int jump_count = 0;
	float jump_cooldown = 0;

	bool on_ground = false;

public:
	UFUNCTION(BlueprintPure, Category = "Movement")
	inline bool OnGround() { return on_ground; }

	UFUNCTION(BlueprintPure, Category = "Player|Movement")
	inline bool CanJump() { return (can_move && jump_count < max_jumps && jump_cooldown <= 0); }


	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Movement")
	float move_speed = 100000.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Movement")
	float jump_force = 40000.0f;


	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;


	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool Jump();

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void IncrementJumpCount() { jump_count++; }

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void AddMovementForce(const FVector input) { if (can_move) additional_force += input; }

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void AddExternalForce(const FVector input) { additional_force += input; }

protected:
	UPrimitiveComponent* affected_body;
	FVector additional_force;
	bool jumped_this_frame = false;

	UFUNCTION(Server, WithValidation, Reliable)
	void HandleMovement(const FVector movement_input, const bool jumped);
	
};
