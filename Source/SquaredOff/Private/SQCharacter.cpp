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
	hit_zone->InitSphereRadius(80.0f);
	hit_zone->SetCollisionProfileName(FName("OverlapAll"));
	hit_zone->SetupAttachment(body);


	USpringArmComponent* camera_arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Arm"));
	camera_arm->TargetArmLength = 300.0f;
	camera_arm->bUsePawnControlRotation = true;
	camera_arm->SetupAttachment(body);
	camera_arm->bEnableCameraLag = true;
	camera_arm->CameraLagSpeed = 15.0f;

	UCameraComponent* camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
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
		HandleMovement(current_movement.ClampMaxSize(1.0f));
		current_movement = FVector::ZeroVector;
	}
}

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

void ASQCharacter::SetupPlayerInputComponent(class UInputComponent* input)
{
	input->BindAxis("Forward", this, &ASQCharacter::Input_Move_Forward);
	input->BindAxis("Strafe", this, &ASQCharacter::Input_Move_Strafe);
	input->BindAxis("Look Yaw", this, &ASQCharacter::Input_Look_Yaw);
	input->BindAxis("Look Pitch", this, &ASQCharacter::Input_Look_Pitch);

	input->BindAction("Jump", IE_Pressed, this, &ASQCharacter::Input_Jump_Press);
	input->BindAction("Jump", IE_Released, this, &ASQCharacter::Input_Jump_Release);

	Super::SetupPlayerInputComponent(input);
}

void ASQCharacter::Input_Move_Forward(float value)
{
	if (value)
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
	if (value)
	{
		FVector direction = UKismetMathLibrary::GetRightVector(GetControlRotation());
		direction.Z = 0;

		if (direction.SizeSquared2D() && direction.Normalize())
			current_movement += direction * value;
	}
}

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

void ASQCharacter::Input_Jump_Press() 
{
	HandleJump();
}

void ASQCharacter::Input_Jump_Release() 
{
}