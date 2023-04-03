// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SPickup.h"
#include "GameplayTagContainer.h"
#include "SPickup_Action.generated.h"


class USAction;


UCLASS()
class ACTIONROGUELIKE_API ASPickup_Action : public ASPickup
{
	GENERATED_BODY()

public:
	ASPickup_Action();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Pickup Attributes")
	TSubclassOf<USAction> ActionClass;

	UPROPERTY(EditDefaultsOnly, Category = "Pickup Attributes")
	float CreditsCost;

	UPROPERTY(EditDefaultsOnly, Category = "Pickup Attributes")
	FGameplayTag ActionTag;

	bool HasAction(TArray<USAction*> Actions);

public:
	
	void Interact_Implementation(APawn* InstigatorPawn);
};
