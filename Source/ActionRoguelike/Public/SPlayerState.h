// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"


class USSaveGame;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChange, ASPlayerState*, PlayerState, float, NewCredits, float, Delta);


UCLASS()
class ACTIONROGUELIKE_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = "OnRep_Credits", Category = "Credits")
	float Credits;

public:

	void SetCredits(float Delta);
	float GetCredits() const;

	void SavePlayerState(USSaveGame* SaveObj);
	void LoadPlayerState(USSaveGame* SaveObj);

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCreditsChange OnCreditsChange;
	
	/** 
	 * OnRep_ can use a parameter containing the old variable it is bound to.
	 * Very useful in this case to figure out the 'Delta'.
	 */
	UFUNCTION()
	void OnRep_Credits(float OldCredits);

	/**
	 * Downside of useing Multicast here is that we send over more data over the net, since it's a RPC with two parameters.
	 * OnRep_ is "free" since Credits is already getting replicated anyway.
	 * 
	 * UFUNCTION(NetMulticast, Reliable)
	 * void MulticastOnCreditsChange(float NewCredits, float Delta);
	 */
};
