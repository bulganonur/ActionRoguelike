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

	ReflectedDamagePercent = 0.2f;
}


void USActionEffect_Thorns::StartAction(AActor* Instigator)
{
	Super::StartAction(Instigator);

	USAttributeComponent* AttributeComp = Instigator->FindComponentByClass<USAttributeComponent>();
	if (AttributeComp)
	{
		AttributeComp->OnHealthChanged.AddDynamic(this, &USActionEffect_Thorns::OnHealthChanged);
	}
}


void USActionEffect_Thorns::StopAction(AActor* Instigator)
{
	USAttributeComponent* AttributeComp = Instigator->FindComponentByClass<USAttributeComponent>();
	if (AttributeComp)
	{
		AttributeComp->OnHealthChanged.RemoveDynamic(this, &USActionEffect_Thorns::OnHealthChanged);
	}

	Super::StopAction(Instigator);
}


void USActionEffect_Thorns::ExecutePeriodicEffect(AActor* Instigator)
{
	UE_LOG(LogTemp, Warning, TEXT("THORNS: %s"), *GetNameSafe(this));

	/*if (Instigator != GetOwningComponent()->GetOwner())
	{
		USGameplayFunctionLibrary::ApplyDamage(GetOwningComponent()->GetOwner(), Instigator, FMath::RoundToFloat(Delta * -ReflectedDamagePercent));

	}*/

	
}


void USActionEffect_Thorns::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (InstigatorActor != OwningComp->GetOwner())
	{
		UE_LOG(LogTemp, Warning, TEXT("THORNS ONHEALTHCHANGED: %s"), *GetNameSafe(this));

		ExecutePeriodicEffect(InstigatorActor);
	}

	UE_LOG(LogTemp, Warning, TEXT("ISACTIONRUNNING: %i"), bIsActionRunning);
	
}
