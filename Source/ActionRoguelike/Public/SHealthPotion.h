// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPickup.h"
#include "SHealthPotion.generated.h"


UCLASS()
class ACTIONROGUELIKE_API ASHealthPotion : public ASPickup
{
	GENERATED_BODY()

public:
	ASHealthPotion();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Potion Attributes")
	float HealAmount;

	UPROPERTY(EditDefaultsOnly, Category = "Potion Attributes")
	float CreditsCost;

public:
	
	void Interact_Implementation(APawn* InstigatorPawn);

	FText GetInteractText_Implementation(APawn* InstigatorPawn) override;
	
};
