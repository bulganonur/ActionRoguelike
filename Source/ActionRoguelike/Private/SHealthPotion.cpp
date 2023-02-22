// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotion.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "SAttributeComponent.h"


ASHealthPotion::ASHealthPotion()
{
	HealAmount = 20.0f;
	ReactivateDelay = 3.0f;
}


void ASHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	
	if (ensure(InstigatorPawn))
	{
		USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (ensure(AttributeComp))
		{
			if (AttributeComp->GetHealth() < AttributeComp->GetHealthMax() && StaticMesh->IsVisible())
			{
				AttributeComp->ApplyHealthChange(this, HealAmount);
				
				Deactivate();
				
				GetWorldTimerManager().SetTimer(TimerHandle_Reactivate, this, &ASHealthPotion::Reactivate, ReactivateDelay);
				UE_LOG(LogTemp, Warning, TEXT("INTERACTION !!!"));
			}
		}
	}
}


void ASHealthPotion::Reactivate()
{
	StaticMesh->SetVisibility(true);
}

void ASHealthPotion::Deactivate()
{
	StaticMesh->SetVisibility(false);
}
