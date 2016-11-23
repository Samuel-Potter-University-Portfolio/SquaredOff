// Fill out your copyright notice in the Description page of Project Settings.

#include "SquaredOff.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "SQCharacter.h"


ASQCharacter::ASQCharacter()
{
	//PrimaryActorTick.bCanEverTick = true;
	//SetReplicateMovement(true);
	//SetReplicates(true);
	//bAlwaysRelevant = true;

	USceneComponent* root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = root;

	body = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	body->SetSimulatePhysics(true);
	body->SetLinearDamping(1.0f);
	body->SetAngularDamping(1.0f);
	body->InitSphereRadius(32.0f);
	body->SetCollisionProfileName(FName("BlockAll"));

	USpringArmComponent* camera_arm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Arm"));
	camera_arm->TargetArmLength = 300.0f;
	camera_arm->bUsePawnControlRotation = true;
	camera_arm->SetupAttachment(body);
	//SpringArm->bEnableCameraLag = true;
	//SpringArm->CameraLagSpeed = 3.0f;

	UCameraComponent* camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(camera_arm);

	//movement = CreateDefaultSubobject<USQMovementComponent>(TEXT("movement"));
	//movement->UpdatedComponent = body;

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void ASQCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASQCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	//USphereComponent* body;
	//movement->AddInputVector(current_movement * movement_force, true);

	//body->AddRelativeLocation(current_movement);

	body->AddForce(current_movement * movement_force);// , NAME_None, true);
	current_movement = FVector::ZeroVector;
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
			//AddMovementInput(direction * movement_force, value, false);
			//movement->AddInputVector(direction * movement_force * value, true);

			UE_LOG(LogInit, Log, TEXT("Test %s"), *(direction * movement_force * value).ToString())
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
		{
			current_movement += direction * value;
			//AddMovementInput(direction * movement_force, value, false);
			//movement->AddInputVector(direction * movement_force * value, true);

			UE_LOG(LogInit, Log, TEXT("Test %s"), *(direction * movement_force * value).ToString())
		}

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
}

void ASQCharacter::Input_Jump_Release() 
{
}