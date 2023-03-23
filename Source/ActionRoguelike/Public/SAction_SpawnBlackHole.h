// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction_SpawnProjectile.h"
#include "SAction_SpawnBlackHole.generated.h"


UCLASS()
class ACTIONROGUELIKE_API USAction_SpawnBlackHole : public USAction_SpawnProjectile
{
	GENERATED_BODY()

public:

	USAction_SpawnBlackHole();

protected:

	void StartAction(AActor* Instigator) override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Rage")
	float RageCost;
};
