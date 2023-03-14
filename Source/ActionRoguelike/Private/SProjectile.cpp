// Fill out your copyright notice in the Description page of Project Settings.


#include "SProjectile.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ASProjectile::ASProjectile()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	RootComponent = SphereComp;
	SphereComp->SetCollisionProfileName("Projectile");

	VFXComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("VFXComp"));
	VFXComp->SetupAttachment(RootComponent);

	SFXComp = CreateDefaultSubobject<UAudioComponent>(TEXT("SFXComp"));
	SFXComp->SetupAttachment(RootComponent);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	MovementComp->InitialSpeed = 8000.0f;
	MovementComp->ProjectileGravityScale = 0.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
}


void ASProjectile::BeginPlay()
{
	Super::BeginPlay();

	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
}


void ASProjectile::Explode()
{
	if (ensure(!IsPendingKill()))
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, VFX_Impact, GetActorLocation(), GetActorRotation());

		UGameplayStatics::PlaySoundAtLocation(this, SFX_Impact, GetActorLocation(), GetActorRotation());
		
		Destroy();
	}
}





