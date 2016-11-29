// SquaredOff - TODO: Write notice

#pragma once

#include "Components/ActorComponent.h"
#include "SQRangedProjectile.h"
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

	UPROPERTY(BlueprintReadOnly, Category = "Player|Attack")
	UPrimitiveComponent* affected_body;

	UPROPERTY(BlueprintReadOnly, Category = "Player|Attack")
	EAttackType current_attack;

	UPROPERTY(BlueprintReadOnly, Category = "Player|Attack")
	float current_charge_rate;

	UPROPERTY(BlueprintReadOnly, Category = "Player|Attack")
	float attack_charge;

	UPROPERTY(BlueprintReadOnly, Category = "Player|Attack")
	float attack_cooldown;

	UFUNCTION(BlueprintPure, Category = "Player|Attack")
	virtual inline bool CanAttack() { return attack_cooldown == 0.0f && current_attack == EAttackType::None; }


	UFUNCTION(BlueprintCallable, Category = "Player|Attack")
	virtual bool BeginAttackCharge(const EAttackType attack_type);
	UFUNCTION(BlueprintCallable, Category = "Player|Attack")
	virtual bool UnleashAttack(const EAttackType attack_type);
	
	/* Dash */
public:
	UPROPERTY(EditDefaultsOnly, Category = "Player|Attack")
	float dash_charge_rate = 3.0f;
	UPROPERTY(BlueprintAssignable, Category = "Player|Attack")
	FAttackFunction OnDashBeginCharge; 
	UPROPERTY(BlueprintAssignable, Category = "Player|Attack")
	FAttackFunction OnDashUnleashCharge;

	void BeginDashCharge();
	void UnleashDash();
	UFUNCTION(Server, Reliable, WithValidation)
	void UnleashDash_Server(const FVector direction, const float dash_amount);

	/* Ranged */
public:
	UPROPERTY(EditDefaultsOnly, Category = "Player|Attack")
	float ranged_charge_rate = 3.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Player|Attack")
	TSubclassOf<ASQRangedProjectile> projectile_type = ASQRangedProjectile::StaticClass();
	UPROPERTY()
	ASQRangedProjectile* projectile;

	UPROPERTY(BlueprintAssignable, Category = "Player|Attack")
	FAttackFunction OnRangedBeginCharge;
	UPROPERTY(BlueprintAssignable, Category = "Player|Attack")
	FAttackFunction OnRangedUnleashCharge;

	void BeginRangedCharge();
	void UnleashRanged();
	UFUNCTION(Server, Reliable, WithValidation)
	void BeginRangedCharge_Server();
	UFUNCTION(Server, Reliable, WithValidation)
	void UnleashRanged_Server(const FVector direction);

	/* Shield */
public:
	UPROPERTY(EditDefaultsOnly, Category = "Player|Attack")
	float shield_duration = 2.0f;
	UPROPERTY(BlueprintAssignable, Category = "Player|Attack")
	FAttackFunction OnShieldStart;
	UPROPERTY(BlueprintAssignable, Category = "Player|Attack")
	FAttackFunction OnShieldEndSuccess;
	UPROPERTY(BlueprintAssignable, Category = "Player|Attack")
	FAttackFunction OnShieldEndFail;

	void ShieldStart();
	void ShieldEnd();
	UFUNCTION(Server, Reliable, WithValidation)
	void ShieldStart_Server();
};
