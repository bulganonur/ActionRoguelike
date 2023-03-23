// Fill out your copyright notice in the Description page of Project Settings.


#include "SDashProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ASDashProjectile::ASDashProjectile()
{
	TeleportDelay = 0.2f;
	DetonateDelay = 0.2f;

	MovementComp->InitialSpeed = 6000.0f;
}


void ASDashProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedDetonate, this, &ASDashProjectile::Explode, DetonateDelay);
}

void ASDashProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASDashProjectile::OnComponentBeginOverlap);
}


void ASDashProjectile::Explode()
{
	/* Clear timer if the Explode was already called through another source like OnComponentHit */
	GetWorldTimerManager().ClearTimer(TimerHandle_DelayedDetonate);

	UGameplayStatics::SpawnEmitterAtLocation(this, VFX_Impact, GetActorLocation(), GetActorRotation());

	VFXComp->DeactivateSystem();

	MovementComp->StopMovementImmediately();
	SetActorEnableCollision(false);

	FTimerHandle TimerHandle_DelayedTeleport;
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedTeleport, this, &ASDashProjectile::TeleportInstigator, TeleportDelay);
}


void ASDashProjectile::TeleportInstigator()
{
	AActor* InstigatorToTeleport = GetInstigator();

	if (ensure(InstigatorToTeleport))
	{
		/* Keep Instigator rotation or it may end up jarring */
		InstigatorToTeleport->TeleportTo(GetActorLocation(), InstigatorToTeleport->GetActorRotation());
	}
	
	Destroy();
}


void ASDashProjectile::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Explode();
}