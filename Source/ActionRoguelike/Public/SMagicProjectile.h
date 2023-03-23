// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectile.h"
#include "SMagicProjectile.generated.h"


class USActionEffect;

UCLASS()
class ACTIONROGUELIKE_API ASMagicProjectile : public ASProjectile
{
	GENERATED_BODY()

public:

	ASMagicProjectile();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float Damage;
	
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCameraShakeBase> CamShake;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	TSubclassOf<USActionEffect> BurningEffectClass;
};
