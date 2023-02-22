// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{
	HealthMax = 100.0f;
	Health = HealthMax;
	
}


bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool USAttributeComponent::IsActorAlive(AActor* Actor)
{
	USAttributeComponent* AttributeComp = GetAttributeComp(Actor);
	if (AttributeComp)
	{
		return AttributeComp->IsAlive();
	}

	return false;
}

float USAttributeComponent::GetHealth()
{
	return Health;
}

float USAttributeComponent::GetHealthMax()
{
	return HealthMax;
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	Health = FMath::Clamp(Health + Delta, 0.0f, HealthMax);

	OnHealthChanged.Broadcast(InstigatorActor, this, Health, Delta);

	return true;
}

USAttributeComponent* USAttributeComponent::GetAttributeComp(AActor* FromActor)
{
	if (FromActor)
	{
		return Cast<USAttributeComponent>(FromActor->GetComponentByClass(USAttributeComponent::StaticClass()));
		/*return FromActor->FindComponentByClass<USAttributeComponent>();*/ // ue5
	}
	
	return nullptr;
}


