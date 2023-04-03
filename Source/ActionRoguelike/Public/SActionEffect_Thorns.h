// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SActionEffect.h"
#include "SActionEffect_Thorns.generated.h"


UCLASS()
class ACTIONROGUELIKE_API USActionEffect_Thorns : public USActionEffect
{
	GENERATED_BODY()

public:

	USActionEffect_Thorns();

protected:

	void StartAction(AActor* Instigator) override;
	void StopAction(AActor* Instigator) override;


	UFUNCTION()
	void OnHealthChange(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewValue, float Delta);

	float ReflectedDamageFraction;
	
};
