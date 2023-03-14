// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SAction_Sprint.generated.h"


UCLASS()
class ACTIONROGUELIKE_API USAction_Sprint : public USAction
{
	GENERATED_BODY()
public:
	
	USAction_Sprint();

protected:

	float SprintBonus;

	void StartAction(AActor* Instigator) override;
	
	void StopAction(AActor* Instigator) override;
};
