// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"
#include "Net/UnrealNetwork.h"
#include "SGameModeBase.h"


static TAutoConsoleVariable<float> CVarDamageMultiplier(TEXT("su.DamageMultiplier"), 1.0f, TEXT("Global damage multiplier for Attribute Component"), ECVF_Cheat);


USAttributeComponent::USAttributeComponent()
{
	HealthMax = 100.0f;
	Health = HealthMax;	

	RageMax = 20.0f;

	SetIsReplicatedByDefault(true);
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


float USAttributeComponent::GetHealth() const
{
	return Health;
}


float USAttributeComponent::GetHealthMax() const
{
	return HealthMax;
}


float USAttributeComponent::GetRage() const
{
	return Rage;
}


float USAttributeComponent::GetRageMax() const
{
	return RageMax;
}


void USAttributeComponent::SetRage(float Delta)
{
	Rage = FMath::Clamp(Rage - Delta, 0.0f, RageMax);

	//OnRageChange.Broadcast(GetOwner(), this, Rage, Delta);

	MulticastOnRageChange(GetOwner(), Rage, Delta);

	UE_LOG(LogTemp, Warning, TEXT("DELTA: %f, RAGE: %f"), Delta, Rage);
}


bool USAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -HealthMax);
}



bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	if (!GetOwner()->CanBeDamaged() && Delta < 0.0f)
	{
		return false;
	}
	
	if (Delta < 0.0f)
	{
		float DamageMultiplier = CVarDamageMultiplier.GetValueOnGameThread();

		Delta *= DamageMultiplier;
	}
	
	
	Health = FMath::Clamp(Health + Delta, 0.0f, HealthMax);

	//OnHealthChange.Broadcast(InstigatorActor, this, Health, Delta);

	if (Health != 0.0f)
	{
		MulticastOnHealthChange(InstigatorActor, Health, Delta);
	}

	if (Delta < 0.0f && Health <= 0.0f)
	{
		GetOwner()->SetCanBeDamaged(false);
		
		ASGameModeBase* GM = GetWorld()->GetAuthGameMode<ASGameModeBase>();
		if (GM)
		{
			GM->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}

	return true;
}


void USAttributeComponent::MulticastOnHealthChange_Implementation(AActor* InstigatorActor, float NewHealth, float Delta)
{
	OnHealthChange.Broadcast(InstigatorActor, this, NewHealth, Delta);
}


void USAttributeComponent::MulticastOnRageChange_Implementation(AActor* InstigatorActor, float NewRage, float Delta)
{
	OnRageChange.Broadcast(InstigatorActor, this, NewRage, Delta);
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


void USAttributeComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAttributeComponent, Health);
	DOREPLIFETIME(USAttributeComponent, HealthMax);

	DOREPLIFETIME(USAttributeComponent, Rage);
	DOREPLIFETIME(USAttributeComponent, RageMax);
}
