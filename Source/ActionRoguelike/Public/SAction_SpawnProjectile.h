// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SAction_SpawnProjectile.generated.h"


UCLASS()
class ACTIONROGUELIKE_API USAction_SpawnProjectile : public USAction
{
	GENERATED_BODY()
	
public:
	
	USAction_SpawnProjectile();

protected:

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(VisibleAnywhere, Category = "Attack")
	FName RHandMuzzleSocket;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* ActionAnim;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	UParticleSystem* VFX_MuzzleFlash;

	float ActionAnimDelay;

	void StartAction(AActor* Instigator) override;

	UFUNCTION()
	void SpawnProjectile(ACharacter* InstigatorChar);
};
