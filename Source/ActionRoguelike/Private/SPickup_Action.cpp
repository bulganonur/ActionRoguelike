// Fill out your copyright notice in the Description page of Project Settings.


#include "SPickup_Action.h"
#include "SAction.h"
#include "SActionComponent.h"
#include "SGameplayTags.h"
#include "SPlayerState.h"


ASPickup_Action::ASPickup_Action()
{
	ReactivateDelay = 2.0f;

	CreditsCost = 2.0f;
}


void ASPickup_Action::Interact_Implementation(APawn* InstigatorPawn)
{
	USActionComponent* ActionComp = InstigatorPawn->FindComponentByClass<USActionComponent>();
	if (ActionComp && !HasAction(ActionComp->GetActions()))
	{
		ASPlayerState* PlayerState = InstigatorPawn->GetPlayerState<ASPlayerState>();
		if (PlayerState && PlayerState->GetCredits() >= CreditsCost && ActionClass)
		{
			ActionComp->AddAction(InstigatorPawn, ActionClass);
			PlayerState->SetCredits(-CreditsCost);

			HideAndCooldown();
			
			UE_LOG(LogTemp, Warning, TEXT("INTERACTION !!!"));
		}
	}
}


bool ASPickup_Action::HasAction(TArray<USAction*> Actions)
{
	for (USAction* Action : Actions)
	{
		if (Action->GetGrantedTags().HasTag(ActionTag))
		{
			return true;
		}
	}
	
	return false;
}