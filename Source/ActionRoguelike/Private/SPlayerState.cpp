// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "SSaveGame.h"

void ASPlayerState::SetCredits(float Delta)
{
	Credits += Delta;

	OnCreditsChange.Broadcast(this, Credits, Delta);

	//MulticastOnCreditsChange(this, Credits, Delta);
}

float ASPlayerState::GetCredits() const
{
	return Credits;
}

void ASPlayerState::SavePlayerState(USSaveGame* SaveObj)
{
	if (SaveObj)
	{
		SaveObj->Credits = Credits;
	}
}

void ASPlayerState::LoadPlayerState(USSaveGame* SaveObj)
{
	if (SaveObj)
	{
		Credits = SaveObj->Credits;
	}
}

void ASPlayerState::OnRep_Credits(float OldCredits)
{
	OnCreditsChange.Broadcast(this, Credits, Credits - OldCredits);
}

//void ASPlayerState::MulticastOnCreditsChange_Implementation(float NewCredits, float Delta)
//{
//	OnCreditsChange.Broadcast(this, NewCredits, Delta);
//}

void ASPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPlayerState, Credits);
}
