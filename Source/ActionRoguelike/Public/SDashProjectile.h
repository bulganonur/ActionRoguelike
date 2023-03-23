// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectile.h"
#include "SDashProjectile.generated.h"


UCLASS()
class ACTIONROGUELIKE_API ASDashProjectile : public ASProjectile
{
	GENERATED_BODY()
	
public:

	ASDashProjectile();

protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float TeleportDelay;
	
	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float DetonateDelay;

	/* Handle to cancel timer if we already hit something */
	FTimerHandle TimerHandle_DelayedDetonate;

	
	void Explode();

	void TeleportInstigator();

	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
