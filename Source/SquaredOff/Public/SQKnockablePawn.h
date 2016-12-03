// SquaredOff - TODO: Write notice

#pragma once

#include "GameFramework/Actor.h"
#include "SQMovementComponent.h"
#include "SQKnockable.h"
#include "SQKnockablePawn.generated.h"

UCLASS()
class SQUAREDOFF_API ASQKnockablePawn : public APawn, public ISQKnockable
{
	GENERATED_BODY()
protected:
	USQMovementComponent* affected_movement;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Knockback")
	float minimum_hit_magnitude = 2300.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Knockback")
	float hit_factor = 1.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Knockback")
	float stun_scale = 0.3f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Knockback")
	float hit_increase_factor = 1.0f;

	float stun_timer;

public:	
	ASQKnockablePawn();
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Knockback")
	bool AttemptKnock(FVector force);

	UFUNCTION(BlueprintCallable, Category = "Knockback")
	void Stun(float time);

	UFUNCTION(BlueprintNativeEvent, Category = "Knockback")
	void OnKnock(FVector force);

	UFUNCTION(BlueprintNativeEvent, Category = "Knockback")
	void OnStun();

	UFUNCTION(BlueprintPure, Category = "Knockback")
	inline bool IsStunned() { return stun_timer > 0; };
	
};
