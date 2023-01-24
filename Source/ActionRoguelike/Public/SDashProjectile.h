// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectile.h"
#include "SDashProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASDashProjectile : public ASProjectile
{
	GENERATED_BODY()
	
public:

	ASDashProjectile();

protected:
	

	virtual void PostInitializeComponents();

	virtual void BeginPlay();

	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* VFX_TeleportIn;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* VFX_TeleportOut;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* VFX_PortalFX;
};
