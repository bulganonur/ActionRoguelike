// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotion.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"

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
		APlayerController* PlayerController = Cast<APlayerController>(InstigatorPawn->GetController());
		
		if (ensure(AttributeComp) && ensure(PlayerController))
		{
			if (AttributeComp->GetHealt() < 100.0f && StaticMesh->IsVisible())
			{
				AttributeComp->ApplyHealthChange(HealAmount);
				
				StaticMesh->SetVisibility(false);
				
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
