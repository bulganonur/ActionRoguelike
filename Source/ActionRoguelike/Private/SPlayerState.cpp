// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"

void ASPlayerState::SetCredits(float Delta)
{
	Credits += Delta;

	OnCreditsChange.Broadcast(this, Credits, Delta);
}

float ASPlayerState::GetCredits() const
{
	return Credits;
}
