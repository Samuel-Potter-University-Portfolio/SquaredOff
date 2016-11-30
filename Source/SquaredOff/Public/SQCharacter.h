// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "SQAttackComponent.h"
#include "SQMovementComponent.h"
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
	
	UPROPERTY(VisibleDefaultsOnly, Category = "Attack")
	USQAttackComponent* attack_component;

	UPROPERTY(VisibleDefaultsOnly, Category = "Movement")
	USQMovementComponent* cube_movement;

	UFUNCTION(BlueprintPure, Category = "Movement")
	inline bool OnGround() { return cube_movement ? cube_movement->OnGround() : false; }

	UFUNCTION(BlueprintPure, Category = "Player|Movement")
	inline bool CanJump() { return cube_movement ? cube_movement->CanJump() : false; }

public:
	UPROPERTY(BlueprintReadWrite, Category = "Input")
	bool invert_look = false;

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

	UFUNCTION(BlueprintNativeEvent, Category = "Player|Input")
	void Input_Dash_Press();
	UFUNCTION(BlueprintNativeEvent, Category = "Player|Input")
	void Input_Dash_Release();

	UFUNCTION(BlueprintNativeEvent, Category = "Player|Input")
	void Input_Ranged_Press();
	UFUNCTION(BlueprintNativeEvent, Category = "Player|Input")
	void Input_Ranged_Release();
};
