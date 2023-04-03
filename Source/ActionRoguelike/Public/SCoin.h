// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPickup.h"
#include "SCoin.generated.h"


UCLASS()
class ACTIONROGUELIKE_API ASCoin : public ASPickup
{
	GENERATED_BODY()

public:
	ASCoin();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Coin Attributes")
	float CreditsIncrease;

public:

	void Interact_Implementation(APawn* InstigatorPawn);
	
};
