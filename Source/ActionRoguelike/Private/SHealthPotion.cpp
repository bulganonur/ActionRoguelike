// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotion.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "SAttributeComponent.h"
#include "SPlayerState.h"


ASHealthPotion::ASHealthPotion()
{
	HealAmount = 20.0f;
	CreditsCost = 1.0f;
	ReactivateDelay = 3.0f;
}


void ASHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if (ensure(InstigatorPawn))
	{
		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributeComp(InstigatorPawn);
		if (ensure(AttributeComp) && AttributeComp->GetHealth() < AttributeComp->GetHealthMax() && StaticMesh->IsVisible())
		{
			ASPlayerState* PlayerState = InstigatorPawn->GetPlayerState<ASPlayerState>();
			if (PlayerState && PlayerState->GetCredits() >= CreditsCost)
			{
				AttributeComp->ApplyHealthChange(this, HealAmount);
				
				PlayerState->SetCredits(-CreditsCost);
						
				Deactivate();
				
				GetWorldTimerManager().SetTimer(TimerHandle_Reactivate, this, &ASHealthPotion::Reactivate, ReactivateDelay);
				UE_LOG(LogTemp, Warning, TEXT("INTERACTION !!!"));
			}
		}
	}
}

