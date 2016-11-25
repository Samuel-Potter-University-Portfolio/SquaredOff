// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "SQMovementComponent.h"
#include "SQCharacter.generated.h"

UCLASS()
class SQUAREDOFF_API ASQCharacter : public APawn
{
	GENERATED_BODY()
private:
	FVector current_movement;
	USphereComponent* body;
	USphereComponent* hit_zone;

	UPROPERTY(EditAnywhere, Category = "Player|Character")
	float movement_force = 130000.0f;


protected:
	UPROPERTY(EditAnywhere, Category = "Player|Character")
	int max_jumps = 2;

	UPROPERTY(BlueprintReadOnly, Category = "Player|Character")
	int jump_count = 0;

	float last_jump_direction = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Player|Character")
	bool on_ground = false;

public:
	UPROPERTY(EditAnywhere, Category = "Player|Character")
	bool invert_look = false;

	UFUNCTION(Server, WithValidation, Reliable)
	void HandleMovement(FVector movement_input);

	UFUNCTION(Server, WithValidation, Reliable)
	void HandleJump();

	ASQCharacter();

	virtual void BeginPlay() override;
	
	virtual void Tick( float DeltaSeconds ) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* input) override;	
	
	virtual void Input_Move_Forward(float value);
	virtual void Input_Move_Strafe(float value);
	virtual void Input_Look_Yaw(float value);
	virtual void Input_Look_Pitch(float value);

	virtual void Input_Jump_Press();
	virtual void Input_Jump_Release();
};
