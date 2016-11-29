// SquaredOff - TODO: Write notice

#pragma once

#include "GameFramework/Actor.h"
#include "SQRangedProjectile.generated.h"

UCLASS()
class SQUAREDOFF_API ASQRangedProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ASQRangedProjectile();

	UPROPERTY(VisibleDefaultsOnly, Category = "Attack")
	USphereComponent* hit_zone;

	UPROPERTY(BlueprintReadOnly, Replicated)
	APawn* parent;

	UPROPERTY(BlueprintReadOnly, Category = "Attack")
	float life_time = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float life_length = 5.0f;

	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Attack")
	float charge = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Attack")
	float charge_rate = 0.0f;


	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION(BlueprintNativeEvent, Category = "Attack")
	void Attach(APawn* pawn);
	UFUNCTION(BlueprintNativeEvent, Category = "Attack")
	void Detach();

	UFUNCTION(BlueprintPure, Category = "Attack")
	inline bool IsAttached() { return attached; }

protected:
	UPROPERTY(Replicated)
	bool attached = true;
};
