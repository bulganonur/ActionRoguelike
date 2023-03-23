// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SActionEffect.h"
#include "SActionEffect_Burning.generated.h"


UCLASS()
class ACTIONROGUELIKE_API USActionEffect_Burning : public USActionEffect
{
	GENERATED_BODY()

public:

	USActionEffect_Burning();

protected:

	void ExecutePeriodicEffect(AActor* Instigator) override;

	float BurningDamage;
	
};
