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

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Movement")
	float move_speed = 100000.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Movement")
	float jump_force = 40000.0f;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	
	void Jump() { if(can_move) jumped_this_frame = true; };

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
