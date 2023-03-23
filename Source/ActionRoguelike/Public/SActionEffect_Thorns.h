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

	void ExecutePeriodicEffect(AActor* Instigator) override;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

	float ReflectedDamagePercent;
	
};
