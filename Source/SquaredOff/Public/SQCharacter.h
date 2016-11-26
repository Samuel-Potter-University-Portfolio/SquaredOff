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

	UPROPERTY(EditAnywhere, Category = "Player|Character")
	USphereComponent* body;

	UPROPERTY(EditAnywhere, Category = "Player|Character")
	USphereComponent* hit_zone;

	UCameraComponent* camera;

	UPROPERTY(EditAnywhere, Category = "Player|Character")
	float movement_force = 110000.0f;

protected:
	UPROPERTY(EditAnywhere, Category = "Player|Character")
	int max_jumps = 2;

	UPROPERTY(BlueprintReadOnly, Category = "Player|Character")
	int jump_count = 0;
	float jump_cooldown = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Player|Character")
	bool on_ground = false;

	UPROPERTY(BlueprintReadWrite, Category = "Player|Character")
	bool can_move = true;

public:
	UPROPERTY(BlueprintReadWrite, Category = "Player|Character")
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

	UFUNCTION(BlueprintNativeEvent, Category = "Player|Character|Input")
	void Input_Jump_Press();
	UFUNCTION(BlueprintNativeEvent, Category = "Player|Character|Input")
	void Input_Jump_Release();

	UFUNCTION(BlueprintPure, Category = "Player|Character")
	inline bool CanJump() { return (jump_count < max_jumps && jump_cooldown <= 0); }


protected:
	UPROPERTY(BlueprintReadOnly, Category = "Player|Character|Attack")
	float attack_cooldown = 0.0f;
	UPROPERTY(EditAnywhere, Category = "Player|Character|Attack")
	float attack_duration = 1.5f;

	UPROPERTY(BlueprintReadOnly, Category = "Player|Character|Attack")
	bool charging_dash = false;
	UPROPERTY(BlueprintReadOnly, Category = "Player|Character|Attack")
	float dash_charge = 0;
	UPROPERTY(EditAnywhere, Category = "Player|Character|Attack")
	float dash_charge_rate = 3.0f;

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Player|Character|Input")
	void Input_Dash_Press();
	UFUNCTION(BlueprintNativeEvent, Category = "Player|Character|Input")
	void Input_Dash_Release();
	UFUNCTION(BlueprintNativeEvent, Category = "Player|Character|Attack")
	void Attack_Dash();
	UFUNCTION(Server, WithValidation, Reliable)
	void Attack_Dash_Server(const FVector direction, const float dash_amount);


	UFUNCTION(BlueprintNativeEvent, Category = "Player|Character|Input")
	void Input_Ranged_Press();
	UFUNCTION(BlueprintNativeEvent, Category = "Player|Character|Input")
	void Input_Ranged_Release();
	UFUNCTION(BlueprintNativeEvent, Category = "Player|Character|Attack")
	void Attack_Ranged();
};
