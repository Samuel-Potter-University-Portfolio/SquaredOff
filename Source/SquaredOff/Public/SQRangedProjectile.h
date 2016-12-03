// SquaredOff - TODO: Write notice

#pragma once

#include "GameFramework/Actor.h"
#include "SQRangedProjectile.generated.h"

UCLASS()
class SQUAREDOFF_API ASQRangedProjectile : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleDefaultsOnly, Category = "Attack")
	USphereComponent* hit_zone;
	UPROPERTY(BlueprintReadOnly, Replicated)
	APawn* parent;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Attack")
	float life_time = 0.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float life_length = 5.0f;


	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Attack")
	float charge = 0.0f;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float base_damage = 2400.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float charge_damage_factor = 1.25f;


public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float charge_rate = 0.0f;


	ASQRangedProjectile();

	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION(BlueprintNativeEvent, Category = "Attack")
	void Attach(APawn* pawn);
	UFUNCTION(BlueprintNativeEvent, Category = "Attack")
	void Detach();

	UFUNCTION(BlueprintPure, Category = "Attack")
	inline bool IsAttached() { return attached; }

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* overlapped_comp, AActor* other_actor, UPrimitiveComponent* other_comp, int32 other_body_index, bool from_sweep, const FHitResult& sweep_result);
	UFUNCTION(BlueprintNativeEvent, Category = "Attack")
	void OnContactKnockable(AActor* actor);

protected:
	UPROPERTY(Replicated)
	bool attached = true;
};
