// Fill out your copyright notice in the Description page of Project Settings.


#include "SCoin.h"
#include "SPlayerState.h"

ASCoin::ASCoin()
{
	CoinIncrease = 1.0f;
	ReactivateDelay = 2.0f;
}

void ASCoin::Interact_Implementation(APawn* InstigatorPawn)
{
	if (InstigatorPawn)
	{
		ASPlayerState* PlayerState = InstigatorPawn->GetPlayerState<ASPlayerState>();
		if (PlayerState && StaticMesh->IsVisible())
		{
			PlayerState->SetCredits(CoinIncrease);

			Deactivate();

			GetWorldTimerManager().SetTimer(TimerHandle_Reactivate, this, &ASCoin::Reactivate, ReactivateDelay);
			UE_LOG(LogTemp, Warning, TEXT("CREDITS: %f"), PlayerState->GetCredits());
		}
	}
}
