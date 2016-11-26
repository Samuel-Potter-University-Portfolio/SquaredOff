// Fill out your copyright notice in the Description page of Project Settings.

#include "SquaredOff.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "SQCharacter.h"


ASQCharacter::ASQCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
	SetReplicateMovement(true);
	bAlwaysRelevant = true;


	body = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	body->SetSimulatePhysics(true);
	body->SetLinearDamping(1.0f);
	body->SetAngularDamping(1.0f);
	body->InitSphereRadius(32.0f);
	body->SetCollisionProfileName(FName("BlockAll"));
	RootComponent = body;

	hit_zone = CreateDefaultSubobject<USphereComponent>(TEXT("Hit Zone"));
	hit_zone->SetSimulatePhysics(false);
	hit_zone->InitSphereRadius(100.0f);
	hit_zone->SetCollisionProfileName(FName("OverlapAll"));
	hit_zone->SetupAttachment(body);


	USpringArmComponent* camera_arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Arm"));
	camera_arm->TargetArmLength = 300.0f;
	camera_arm->bUsePawnControlRotation = true;
	camera_arm->SetupAttachment(body);
	//camera_arm->bEnableCameraLag = true;
	//camera_arm->CameraLagSpeed = 15.0f;

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(camera_arm);
	

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void ASQCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASQCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	
	if (!current_movement.IsNearlyZero())
	{
		HandleMovement(current_movement.GetClampedToMaxSize(1.0f));
		current_movement = FVector::ZeroVector;
	}

	//Check if on ground (TODO - Fix for double jumps)
	{
		FHitResult hit;
		FCollisionQueryParams params;
		params.AddIgnoredActor(this);
		FVector location = GetActorLocation();

		if (jump_cooldown > 0)
			jump_cooldown -= DeltaTime;

		on_ground = GetWorld()->LineTraceSingleByChannel(
			hit, 
			location,
			location + FVector(0,0,-100),
			ECC_WorldStatic,
			params
		);

		if (on_ground)
			jump_count = 0;
		else if (jump_count == 0)
			jump_count = 1;
	}

	//Attacks
	if (attack_cooldown > 0)
		attack_cooldown -= DeltaTime;

	else if(charging_attack)
	{
		//Dash
		if (dash_charge)
		{
			dash_charge += DeltaTime / dash_charge_rate;

			if (dash_charge >= 1.0f)
				dash_charge = 1.0f;
		}

		//Ranged
		if (ranged_charge)
		{
			ranged_charge += DeltaTime / ranged_charge_rate;

			if (ranged_charge >= 1.0f)
				ranged_charge = 1.0f;
		}
	}
}

void ASQCharacter::SetupPlayerInputComponent(class UInputComponent* input)
{
	input->BindAxis("Forward", this, &ASQCharacter::Input_Move_Forward);
	input->BindAxis("Strafe", this, &ASQCharacter::Input_Move_Strafe);
	input->BindAxis("Look Yaw", this, &ASQCharacter::Input_Look_Yaw);
	input->BindAxis("Look Pitch", this, &ASQCharacter::Input_Look_Pitch);

	input->BindAction("Jump", IE_Pressed, this, &ASQCharacter::Input_Jump_Press);
	input->BindAction("Jump", IE_Released, this, &ASQCharacter::Input_Jump_Release);

	input->BindAction("Dash", IE_Pressed, this, &ASQCharacter::Input_Dash_Press);
	input->BindAction("Dash", IE_Released, this, &ASQCharacter::Input_Dash_Release);

	input->BindAction("Ranged", IE_Pressed, this, &ASQCharacter::Input_Ranged_Press);
	input->BindAction("Ranged", IE_Released, this, &ASQCharacter::Input_Ranged_Release);

	Super::SetupPlayerInputComponent(input);
}

/*Server-side movement*/

void ASQCharacter::HandleMovement_Implementation(FVector movement_input)
{
	body->AddForce(movement_input * movement_force);// , NAME_None, true);
}

bool ASQCharacter::HandleMovement_Validate(FVector movement_input)
{
	return Role >= ROLE_AutonomousProxy;
}

void ASQCharacter::HandleJump_Implementation()
{
	body->AddForce(FVector(0.0f, 0.0f, 100000.0f), NAME_None, true);
}

bool ASQCharacter::HandleJump_Validate()
{
	return Role >= ROLE_AutonomousProxy;
}

/*Movement*/

void ASQCharacter::Input_Move_Forward(float value)
{
	if (value && can_move)
	{
		FVector direction = UKismetMathLibrary::GetForwardVector(GetControlRotation());
		direction.Z = 0;

		if (direction.SizeSquared2D() && direction.Normalize())
		{
			current_movement += direction * value;
		}
	}
}

void ASQCharacter::Input_Move_Strafe(float value) 
{
	if (value && can_move)
	{
		FVector direction = UKismetMathLibrary::GetRightVector(GetControlRotation());
		direction.Z = 0;

		if (direction.SizeSquared2D() && direction.Normalize())
			current_movement += direction * value;
	}
}

/*Looking*/

void ASQCharacter::Input_Look_Yaw(float value) 
{
	if (value)
		AddControllerYawInput(value);
}

void ASQCharacter::Input_Look_Pitch(float value) 
{
	if (value)
		AddControllerPitchInput(invert_look ? value : -value);
}

/*Jumping*/

void ASQCharacter::Input_Jump_Press_Implementation()
{
	if (CanJump())
	{
		jump_count++;
		jump_cooldown = 0.3;
		on_ground = false;
		HandleJump();
	}
}

void ASQCharacter::Input_Jump_Release_Implementation()
{
}

/*Dash*/

void ASQCharacter::Input_Dash_Press_Implementation()
{
	if (CanJump() && !charging_attack)
	{
		charging_attack = true;
		dash_charge = 0.001f;
		ranged_charge = 0;
	}
}

void ASQCharacter::Input_Dash_Release_Implementation()
{
	if(attack_cooldown <= 0 && IsChargingDash())
		Attack_Dash();

	else if (IsChargingDash())
		charging_attack = false;
}

void ASQCharacter::Attack_Dash_Implementation()
{
	Attack();
	jump_count++;

	FVector direction = UKismetMathLibrary::GetForwardVector(camera->GetComponentRotation());
	Attack_Dash_Server(direction, dash_charge);
}

void ASQCharacter::Attack_Dash_Server_Implementation(const FVector direction, const float dash_amount)
{
	body->AddForce(direction * (10000000.0f + 5000000.0f * dash_amount));
}

bool ASQCharacter::Attack_Dash_Server_Validate(const FVector direction, const float dash_amount)
{
	return Role >= ROLE_AutonomousProxy;
}

/*Ranged*/

void ASQCharacter::Input_Ranged_Press_Implementation()
{
	if (attack_cooldown <= 0 && !charging_attack)
	{
		charging_attack = true;
		ranged_charge = 0.001f;
		dash_charge = 0;
	}
}

void ASQCharacter::Input_Ranged_Release_Implementation()
{
	if (IsChargingRanged())
		Attack_Ranged();
}

void ASQCharacter::Attack_Ranged_Implementation()
{
	Attack();
}