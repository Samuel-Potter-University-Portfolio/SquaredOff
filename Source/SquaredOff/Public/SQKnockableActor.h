// SquaredOff - TODO: Write notice

#pragma once

#include "GameFramework/Actor.h"
#include "SQMovementComponent.h"
#include "SQKnockable.h"
#include "SQKnockableActor.generated.h"

UCLASS()
class SQUAREDOFF_API ASQKnockableActor : public AActor, public ISQKnockable
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Knockback")
	UPrimitiveComponent* affected_body;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Knockback")
	float minimum_hit_magnitude = 2300.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Knockback")
	float hit_factor = 1.0f;

	UFUNCTION(Server, WithValidation, Reliable)
	void AddForce(const FVector force);

public:	
	ASQKnockableActor();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Knockback")
	bool AttemptKnock(FVector force);

	UFUNCTION(BlueprintNativeEvent, Category = "Knockback")
	void OnKnock(FVector force);
	
};
