// SquaredOff - TODO: Write notice

#include "SquaredOff.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "SQAttackComponent.h"


USQAttackComponent::USQAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USQAttackComponent::BeginPlay()
{
	Super::BeginPlay();	
}

void USQAttackComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if (attack_cooldown > 0)
	{
		attack_cooldown -= DeltaTime;
		return;
	}
	else
		attack_cooldown = 0;

	if (current_attack == EAttackType::None)
		return;

	attack_charge += DeltaTime / current_charge_rate;

	if (attack_charge > 1.0f)
		attack_charge = 1.0f;
}

bool USQAttackComponent::BeginAttackCharge(const EAttackType attack_type)
{
	if (current_attack != EAttackType::None)
		return false;

	current_attack = attack_type;

	switch (attack_type)
	{
	case EAttackType::Dash:
		BeginDashCharge();
		break;
	case EAttackType::Ranged:
		break;
	case EAttackType::Item:
		break;
	case EAttackType::Shield:
		break;
	};

	return true;
}

bool USQAttackComponent::UnleashAttack(const EAttackType attack_type)
{
	if (current_attack == EAttackType::None || current_attack != attack_type)
		return false;
	
	//Lets player still queue up attacks even if can't be used at time
	if (attack_cooldown == 0.0f)
	{
		switch (attack_type)
		{
		case EAttackType::Dash:
			UnleashDash();
			break;
		case EAttackType::Ranged:
			break;
		case EAttackType::Item:
			break;
		case EAttackType::Shield:
			break;
		};
		attack_cooldown = 1.5;
		attack_charge = 0.0f;
	}

	current_attack = EAttackType::None;
	return true;
}

/* DASH */

void USQAttackComponent::BeginDashCharge()
{
	current_charge_rate = dash_charge_rate;

	if (OnDashBeginCharge.IsBound())
		OnDashBeginCharge.Broadcast();
}

void USQAttackComponent::UnleashDash()
{
	APawn* owner = (APawn*)GetOwner();
	FVector direction = UKismetMathLibrary::GetForwardVector(owner->GetControlRotation());
	UnleashDash_Server(direction, attack_charge);

	if (OnDashUnleashCharge.IsBound())
		OnDashUnleashCharge.Broadcast();
}

void USQAttackComponent::UnleashDash_Server_Implementation(const FVector direction, const float dash_amount)
{
	affected_body->AddForce(direction * (10000000.0f + 5000000.0f * dash_amount));
}

bool USQAttackComponent::UnleashDash_Server_Validate(const FVector direction, const float dash_amount)
{
	return true;
}