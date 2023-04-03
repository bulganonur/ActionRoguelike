// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionEffect_Thorns.h"
#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "SGameplayFunctionLibrary.h"
#include "SGameplayTags.h"


USActionEffect_Thorns::USActionEffect_Thorns()
{
	GrantedTags.AddTag(STATUS_THORNS);

	bIsAutoStartOn = true;

	Duration = 0.0f;

	Period = 0.0f;

	ReflectedDamageFraction = 0.2f;
}


void USActionEffect_Thorns::StartAction(AActor* Instigator)
{
	Super::StartAction(Instigator);

	USAttributeComponent* AttributeComp = Instigator->FindComponentByClass<USAttributeComponent>();
	if (AttributeComp)
	{
		/** Start listening */
		AttributeComp->OnHealthChange.AddDynamic(this, &USActionEffect_Thorns::OnHealthChange);
	}
}


void USActionEffect_Thorns::StopAction(AActor* Instigator)
{
	Super::StopAction(Instigator);

	USAttributeComponent* AttributeComp = Instigator->FindComponentByClass<USAttributeComponent>();
	if (AttributeComp)
	{
		/** Stop listening */
		AttributeComp->OnHealthChange.RemoveDynamic(this, &USActionEffect_Thorns::OnHealthChange);
	}
}


void USActionEffect_Thorns::OnHealthChange(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewValue, float Delta)
{
	if (Delta < 0.0f && InstigatorActor != OwningComp->GetOwner())
	{
		USGameplayFunctionLibrary::ApplyDamage(OwningComp->GetOwner(), InstigatorActor, FMath::RoundToFloat(Delta * -ReflectedDamageFraction));
	}
}
