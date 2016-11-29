// SquaredOff - TODO: Write notice

#include "SquaredOff.h"
#include "Net/UnrealNetwork.h"
#include "SQRangedProjectile.h"


ASQRangedProjectile::ASQRangedProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
	SetReplicateMovement(true);
	bAlwaysRelevant = true;

	hit_zone = CreateDefaultSubobject<USphereComponent>(TEXT("Hit Zone"));
	hit_zone->SetSimulatePhysics(true);
	hit_zone->SetEnableGravity(false);
	hit_zone->InitSphereRadius(65.0f);
	hit_zone->SetCollisionProfileName(FName("OverlapAll"));
	//hit_zone->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RootComponent = hit_zone;
}

void ASQRangedProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASQRangedProjectile::Tick( float DeltaTime )
{
	Super::Tick(DeltaTime);
	
	if (Role == ROLE_Authority)
	{
		if (attached && parent)
		{
			charge += DeltaTime / charge_rate;
			if (charge > 1.0f)
				charge = 1.0f;
			life_time = life_length;
		}
		else
		{
			life_time -= DeltaTime;

			if (life_time < 0)
				Destroy();
		}
	}
	else 
	{
		if(attached && parent)
			SetActorLocation(parent->GetActorLocation());
	}
}

void ASQRangedProjectile::Attach_Implementation(APawn* pawn) 
{
	parent = pawn;
	attached = true;
}

void ASQRangedProjectile::Detach_Implementation() 
{
	attached = false;
}

void ASQRangedProjectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASQRangedProjectile, attached);
	DOREPLIFETIME(ASQRangedProjectile, charge);
	DOREPLIFETIME(ASQRangedProjectile, parent);
}