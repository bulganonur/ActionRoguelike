// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChange, ASPlayerState*, PlayerState, float, NewCredits, float, Delta);

UCLASS()
class ACTIONROGUELIKE_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Credits")
	float Credits;

public:

	void SetCredits(float Delta);
	float GetCredits() const;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCreditsChange OnCreditsChange;
	
};
