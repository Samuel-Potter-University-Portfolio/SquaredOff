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
	camera_arm->SetRelativeLocation(FVector(0, 0, 50.0f));
	//camera_arm->bEnableCameraLag = true;
	//camera_arm->CameraLagSpeed = 15.0f;

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(camera_arm);
	
	attack_component = CreateDefaultSubobject<USQAttackComponent>(TEXT("Attack Component"));
	attack_component->affected_body = body;

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void ASQCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASQCharacter::Tick( float DeltaTime )
{
	Super::Tick(DeltaTime);

	if (Role != ROLE_AutonomousProxy)
		return;
	
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
	return true;
}

void ASQCharacter::HandleJump_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, *FString::Printf(TEXT("WooP")));
	body->AddForce(FVector(0.0f, 0.0f, 100000.0f), NAME_None, true);
}

bool ASQCharacter::HandleJump_Validate()
{
	return true;
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

/* Attack inputs*/

/* Dash */

void ASQCharacter::Input_Dash_Press_Implementation()
{
	if (CanJump() && attack_component->BeginAttackCharge(EAttackType::Dash))
		jump_count++;
}

void ASQCharacter::Input_Dash_Release_Implementation()
{
	attack_component->UnleashAttack(EAttackType::Dash);
}

/* Ranged */

void ASQCharacter::Input_Ranged_Press_Implementation()
{
	attack_component->BeginAttackCharge(EAttackType::Ranged);
}

void ASQCharacter::Input_Ranged_Release_Implementation()
{
	attack_component->UnleashAttack(EAttackType::Ranged);
}