// SquaredOff - TODO: Write notice

#pragma once

#include "Components/ActorComponent.h"
#include "SQRangedProjectile.h"
#include "SQMovementComponent.h"
#include "SQAttackComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttackFunction);


UENUM(BlueprintType)
enum class EAttackType : uint8
{
	None, Dash, Ranged, Item, Shield
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SQUAREDOFF_API USQAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USQAttackComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	UPROPERTY(BlueprintReadOnly, Category = "Attack")
	UPrimitiveComponent* affected_body;

	UPROPERTY(BlueprintReadOnly, Category = "Attack")
	USQMovementComponent* affected_movement;

	UPROPERTY(BlueprintReadOnly, Category = "Attack")
	EAttackType current_attack;

	UPROPERTY(BlueprintReadOnly, Category = "Attack")
	float current_charge_rate;

	UPROPERTY(BlueprintReadOnly, Category = "Attack")
	float attack_charge;

	UPROPERTY(BlueprintReadOnly, Category = "Attack")
	float attack_cooldown;

	UFUNCTION(BlueprintPure, Category = "Attack")
	virtual inline bool CanAttack() { return attack_cooldown == 0.0f && current_attack == EAttackType::None; }


	UFUNCTION(BlueprintCallable, Category = "Attack")
	virtual bool BeginAttackCharge(const EAttackType attack_type);
	UFUNCTION(BlueprintCallable, Category = "Attack")
	virtual bool UnleashAttack(const EAttackType attack_type);
	
	/* Dash */
public:
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float dash_charge_rate = 3.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float min_dash_force = 170000.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float max_dash_force = 320000.0f;
	UPROPERTY(BlueprintAssignable, Category = "Attack")
	FAttackFunction OnDashBeginCharge; 
	UPROPERTY(BlueprintAssignable, Category = "Attack")
	FAttackFunction OnDashUnleashCharge;

	void BeginDashCharge();
	void UnleashDash();

	/* Ranged */
public:
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float ranged_charge_rate = 3.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	TSubclassOf<ASQRangedProjectile> projectile_type = ASQRangedProjectile::StaticClass();
	UPROPERTY()
	ASQRangedProjectile* projectile;

	UPROPERTY(BlueprintAssignable, Category = "Attack")
	FAttackFunction OnRangedBeginCharge;
	UPROPERTY(BlueprintAssignable, Category = "Attack")
	FAttackFunction OnRangedUnleashCharge;

	void BeginRangedCharge();
	void UnleashRanged();
	UFUNCTION(Server, Reliable, WithValidation)
	void BeginRangedCharge_Server();
	UFUNCTION(Server, Reliable, WithValidation)
	void UnleashRanged_Server(const FVector direction);

	/* Shield */
public:
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float shield_duration = 2.0f;
	UPROPERTY(BlueprintAssignable, Category = "Attack")
	FAttackFunction OnShieldStart;
	UPROPERTY(BlueprintAssignable, Category = "Attack")
	FAttackFunction OnShieldEndSuccess;
	UPROPERTY(BlueprintAssignable, Category = "Attack")
	FAttackFunction OnShieldEndFail;

	void ShieldStart();
	void ShieldEnd();
	UFUNCTION(Server, Reliable, WithValidation)
	void ShieldStart_Server();
};
