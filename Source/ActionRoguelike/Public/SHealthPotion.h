// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPickup.h"
#include "SHealthPotion.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASHealthPotion : public ASPickup
{
	GENERATED_BODY()

public:
	ASHealthPotion();

protected:

	float HealAmount;
	float ReactivateDelay;

	FTimerHandle TimerHandle_Reactivate;

	void Reactivate();


public:
	
	void Interact_Implementation(APawn* InstigatorPawn);
	
};
