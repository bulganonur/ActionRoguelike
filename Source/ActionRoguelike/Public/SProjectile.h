// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SProjectile.generated.h"

class UAudioComponent;
class UParticleSystemComponent;
class UProjectileMovementComponent;
class USphereComponent;

UCLASS(ABSTRACT) // "ABSTRACT" marks this class as incomplete, keeping this out of certain drop-down windows like SpawnActor in Unreal Editor
class ACTIONROGUELIKE_API ASProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASProjectile();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* SphereComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* MovementComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UParticleSystemComponent* VFXComp;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	UParticleSystem* VFX_Impact;

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	UAudioComponent* SFXComp;

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	USoundBase* SFX_Impact;
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Explode();

	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;
};
