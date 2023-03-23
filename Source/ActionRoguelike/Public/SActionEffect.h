// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SActionEffect.generated.h"


UCLASS()
class ACTIONROGUELIKE_API USActionEffect : public USAction
{
	GENERATED_BODY()

public:

	USActionEffect();

protected:
	
	virtual void StartAction(AActor* Instigator) override;
	virtual void StopAction(AActor* Instigator) override;

	UFUNCTION(BlueprintCallable, Category = "Effect")
	virtual void ExecutePeriodicEffect(AActor* Instigator);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Duration;
	
	/** Time between 'ticks' to apply effect */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Period;

	FTimerHandle TimerHandle_Duration;
	FTimerHandle TimerHandle_Period;
	
};
