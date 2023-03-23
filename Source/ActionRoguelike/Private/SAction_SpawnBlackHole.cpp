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
	if (AttributeComp && AttributeComp->GetRage() >= RageCost)
	{
		Super::StartAction(Instigator);

		AttributeComp->SetRage(RageCost);
	}
}
