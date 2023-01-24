// Fill out your copyright notice in the Description page of Project Settings.


#include "SDashProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ASDashProjectile::ASDashProjectile()
{
	MovementComp->InitialSpeed = 10000.0f;
}

void ASDashProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	SphereComp->OnComponentHit.AddDynamic(this, &ASDashProjectile::OnComponentHit);
}

void ASDashProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
	
	MovementComp->ProjectileGravityScale = 0.0f;
}

void ASDashProjectile::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), VFX_TeleportOut, GetActorLocation());
	SetActorHiddenInGame(true);
	MovementComp->StopMovementImmediately();
}
