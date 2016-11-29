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

	//GEngine->AddOnScreenDebugMessage(-1, DeltaTime, FColor::White, *FString::Printf(TEXT("Woo %i"), (int)GetOwnerRole()));

	if (attack_cooldown > 0)
	{
		attack_cooldown -= DeltaTime;
		return;
	}
	else
		attack_cooldown = 0;

	if (current_attack == EAttackType::None)
		return;

	if (current_attack == EAttackType::Shield)
	{
		attack_charge -= DeltaTime;

		if (attack_charge <= 0)
		{
			current_attack = EAttackType::None;
			attack_cooldown = 1.0f;
			attack_charge = 0.0f;

			ShieldEnd();
		}
		return;
	}
	

	attack_charge += DeltaTime / current_charge_rate;

	if (attack_charge > 1.0f)
		attack_charge = 1.0f;
}

bool USQAttackComponent::BeginAttackCharge(const EAttackType attack_type)
{
	if (current_attack != EAttackType::None || attack_cooldown > 0.0f)
		return false;

	current_attack = attack_type;

	switch (attack_type)
	{
	case EAttackType::Dash:
		BeginDashCharge();
		break;
	case EAttackType::Ranged:
		BeginRangedCharge();
		break;
	case EAttackType::Item:
		break;
	case EAttackType::Shield:
		ShieldStart();
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
			UnleashRanged();
			break;
		case EAttackType::Item:
			break;
		case EAttackType::Shield:
			break;
		};
		attack_cooldown = 1.0f;
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

/* RANGED */

void USQAttackComponent::BeginRangedCharge()
{
	current_charge_rate = ranged_charge_rate;
	
	BeginRangedCharge_Server();

	if (OnRangedBeginCharge.IsBound())
		OnRangedBeginCharge.Broadcast();
}

void USQAttackComponent::UnleashRanged()
{
	APawn* owner = (APawn*)GetOwner();
	FVector direction = UKismetMathLibrary::GetForwardVector(owner->GetControlRotation());

	UnleashRanged_Server(direction);
	
	if (OnRangedUnleashCharge.IsBound())
		OnRangedUnleashCharge.Broadcast();
}

void USQAttackComponent::BeginRangedCharge_Server_Implementation()
{
	APawn* owner = (APawn*)GetOwner();

	if (GetWorld() && owner)
	{
		FVector location = owner->GetActorLocation();
		FRotator rotation(0, 0, 0);
		FActorSpawnParameters params;
		projectile = GetWorld()->SpawnActor<ASQRangedProjectile>(projectile_type, location, rotation, params);

		projectile->Attach(owner);
		projectile->charge_rate = ranged_charge_rate;
	}
}

bool USQAttackComponent::BeginRangedCharge_Server_Validate()
{
	return true;
}

void USQAttackComponent::UnleashRanged_Server_Implementation(const FVector direction)
{
	if (projectile)
	{
		projectile->Detach();
		projectile->hit_zone->AddForce(direction * 60000000.0f);
	}
}

bool USQAttackComponent::UnleashRanged_Server_Validate(const FVector direction)
{
	return true;
}

/* SHIELD */

void USQAttackComponent::ShieldStart()
{
	attack_charge = shield_duration;

	if (OnShieldStart.IsBound())
		OnShieldStart.Broadcast();
}

void USQAttackComponent::ShieldEnd()
{
	if (OnShieldEndSuccess.IsBound())
		OnShieldEndSuccess.Broadcast();
	if (OnShieldEndFail.IsBound())
		OnShieldEndFail.Broadcast();
}

void USQAttackComponent::ShieldStart_Server_Implementation()
{
}

bool USQAttackComponent::ShieldStart_Server_Validate()
{
	return true;
}