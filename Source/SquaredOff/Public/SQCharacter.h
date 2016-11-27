// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "SQAttackComponent.h"
#include "SQCharacter.generated.h"

UCLASS()
class SQUAREDOFF_API ASQCharacter : public APawn
{
	GENERATED_BODY()
private:
	FVector current_movement;

	UPROPERTY(VisibleDefaultsOnly, Category = "Player")
	USphereComponent* body;

	UPROPERTY(VisibleDefaultsOnly, Category = "Player")
	USphereComponent* hit_zone;

	UPROPERTY(VisibleDefaultsOnly, Category = "Player")
	UCameraComponent* camera;

	float movement_force = 100000.0f;

	UPROPERTY(VisibleDefaultsOnly, Category = "Player|Attack")
	USQAttackComponent* attack_component;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Player|Movement")
	int max_jumps = 2;

	UPROPERTY(BlueprintReadOnly, Category = "Player|Movement")
	int jump_count = 0;
	float jump_cooldown = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Player|Movement")
	bool on_ground = false;

	UPROPERTY(BlueprintReadWrite, Category = "Player|Movement")
	bool can_move = true;

public:
	UPROPERTY(BlueprintReadWrite, Category = "Player|Input")
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

	UFUNCTION(BlueprintNativeEvent, Category = "Player|Input")
	void Input_Jump_Press();
	UFUNCTION(BlueprintNativeEvent, Category = "Player|Input")
	void Input_Jump_Release();

	UFUNCTION(BlueprintPure, Category = "Player|Movement")
	inline bool CanJump() { return (jump_count < max_jumps && jump_cooldown <= 0); }

	UFUNCTION(BlueprintNativeEvent, Category = "Player|Input")
	void Input_Dash_Press();
	UFUNCTION(BlueprintNativeEvent, Category = "Player|Input")
	void Input_Dash_Release();

	UFUNCTION(BlueprintNativeEvent, Category = "Player|Input")
	void Input_Ranged_Press();
	UFUNCTION(BlueprintNativeEvent, Category = "Player|Input")
	void Input_Ranged_Release();
};
