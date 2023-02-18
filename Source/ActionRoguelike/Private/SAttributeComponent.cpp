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

float USAttributeComponent::GetHealth()
{
	return Health;
}

float USAttributeComponent::GetHealthMax()
{
	return HealthMax;
}

bool USAttributeComponent::ApplyHealthChange(float Delta)
{
	Health = FMath::Clamp(Health + Delta, 0.0f, HealthMax);

	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);

	return true;
}

