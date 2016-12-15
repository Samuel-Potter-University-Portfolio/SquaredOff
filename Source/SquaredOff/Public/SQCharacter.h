// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "SQAttackComponent.h"
#include "SQMovementComponent.h"
#include "SQKnockablePawn.h"
#include "SQCharacter.generated.h"

UCLASS()
class SQUAREDOFF_API ASQCharacter : public ASQKnockablePawn
{
	GENERATED_BODY()
protected:
	FVector current_movement;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Player")
	USphereComponent* body;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Player")
	USphereComponent* hit_zone;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Player")
	UCameraComponent* camera;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Attack")
	USQAttackComponent* attack_component;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Movement")
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
	virtual void Destroyed() override;

	/* Movement */

	virtual void Input_Move_Forward(float value);
	virtual void Input_Move_Strafe(float value);
	virtual void Input_Look_Yaw(float value);
	virtual void Input_Look_Pitch(float value);

	UFUNCTION(BlueprintNativeEvent, Category = "Player|Input")
	void Input_Jump_Press();
	UFUNCTION(BlueprintNativeEvent, Category = "Player|Input")
	void Input_Jump_Release();

	/* Attacks */

	UFUNCTION(BlueprintNativeEvent, Category = "Player|Input")
	void Input_Dash_Press();
	UFUNCTION(BlueprintNativeEvent, Category = "Player|Input")
	void Input_Dash_Release();

	UFUNCTION(BlueprintNativeEvent, Category = "Player|Input")
	void Input_Ranged_Press();
	UFUNCTION(BlueprintNativeEvent, Category = "Player|Input")
	void Input_Ranged_Release();

	/* Damage */

	AActor* last_damage_source;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* overlapped_comp, AActor* other_actor, UPrimitiveComponent* other_comp, int32 other_body_index, bool from_sweep, const FHitResult& sweep_result);
	UFUNCTION(BlueprintNativeEvent, Category = "Attack")
	void OnContactKnockable(AActor* actor);
};
