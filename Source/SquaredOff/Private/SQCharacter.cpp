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

	hit_increase_factor = 0.07f;

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
	hit_zone->OnComponentBeginOverlap.AddDynamic(this, &ASQCharacter::OnBeginOverlap);
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

	cube_movement = CreateDefaultSubobject<USQMovementComponent>(TEXT("Movement Component"));
	cube_movement->UpdatedComponent = body;
	affected_movement = cube_movement;

	attack_component = CreateDefaultSubobject<USQAttackComponent>(TEXT("Attack Component"));
	attack_component->affected_body = body;
	attack_component->affected_movement = cube_movement;

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void ASQCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASQCharacter::Tick( float DeltaTime )
{
	Super::Tick(DeltaTime);
	
	//if (Role < ROLE_AutonomousProxy)
	//	return;
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

/*Movement*/

void ASQCharacter::Input_Move_Forward(float value)
{
	if (value && !IsStunned())
	{
		FVector direction = UKismetMathLibrary::GetForwardVector(GetControlRotation());
		direction.Z = 0;

		if (direction.SizeSquared2D() && direction.Normalize())
			AddMovementInput(direction * value);
	}
}

void ASQCharacter::Input_Move_Strafe(float value) 
{
	if (value && !IsStunned())
	{
		FVector direction = UKismetMathLibrary::GetRightVector(GetControlRotation());
		direction.Z = 0;

		if (direction.SizeSquared2D() && direction.Normalize())
			AddMovementInput(direction * value);
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
	if(!IsStunned())
		cube_movement->Jump();
}

void ASQCharacter::Input_Jump_Release_Implementation()
{
}

/* Attack inputs*/

/* Dash */

void ASQCharacter::Input_Dash_Press_Implementation()
{
	if (CanJump() && !IsStunned() && attack_component->BeginAttackCharge(EAttackType::Dash))
		cube_movement->IncrementJumpCount();
}

void ASQCharacter::Input_Dash_Release_Implementation()
{
	if(!IsStunned())
		attack_component->UnleashAttack(EAttackType::Dash);
}

/* Ranged */

void ASQCharacter::Input_Ranged_Press_Implementation()
{
	if(!IsStunned())
		attack_component->BeginAttackCharge(EAttackType::Ranged);
}

void ASQCharacter::Input_Ranged_Release_Implementation()
{
	if (!IsStunned())
		attack_component->UnleashAttack(EAttackType::Ranged);
}

/* Damage */

void ASQCharacter::OnBeginOverlap(UPrimitiveComponent* overlapped_comp, AActor* other_actor, UPrimitiveComponent* other_comp, int32 other_body_index, bool from_sweep, const FHitResult& sweep_result)
{
	if (!other_actor->GetClass()->ImplementsInterface(USQKnockable::StaticClass()))
		return;

	if (!IsStunned() && other_actor != this)
		OnContactKnockable(other_actor);
}

void ASQCharacter::OnContactKnockable_Implementation(AActor* actor)
{
	if (!actor->GetClass()->ImplementsInterface(USQKnockable::StaticClass()))
		return;

	FVector velocity = GetVelocity();
	const float power = velocity.Size();
	velocity.Normalize();

	FVector to_other = actor->GetActorLocation() - GetActorLocation();
	to_other.Normalize();

	const float velocity_weight = 0.5f;

	const FVector direction = velocity * velocity_weight + to_other * (1.0f - velocity_weight);
	const FVector force = direction * power;
	ISQKnockable::Execute_AttemptKnock(actor, force);
}