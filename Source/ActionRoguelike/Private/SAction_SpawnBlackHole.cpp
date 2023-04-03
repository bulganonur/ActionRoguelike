// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_SpawnBlackHole.h"
#include "SAttributeComponent.h"

USAction_SpawnBlackHole::USAction_SpawnBlackHole()
{
	RageCost = 5.0f;
}

void USAction_SpawnBlackHole::StartAction(AActor* Instigator)
{
	USAttributeComponent* AttributeComp = Instigator->FindComponentByClass<USAttributeComponent>();
	if (AttributeComp)
	{
		Super::StartAction(Instigator);
		
		AttributeComp->SetRage(RageCost);
	}
}

bool USAction_SpawnBlackHole::CanStart(AActor* Instigator)
{
	if (!Super::CanStart(Instigator))
	{
		return false;
	}

	USAttributeComponent* AttributeComp = Instigator->FindComponentByClass<USAttributeComponent>();
	if (AttributeComp && AttributeComp->GetRage() >= RageCost)
	{
		return true;
	}
	
	return false;
}
