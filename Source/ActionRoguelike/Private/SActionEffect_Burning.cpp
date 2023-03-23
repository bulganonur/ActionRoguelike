// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionEffect_Burning.h"
#include "SActionComponent.h"
#include "SGameplayFunctionLibrary.h"
#include "SGameplayTags.h"


USActionEffect_Burning::USActionEffect_Burning()
{
	GrantedTags.AddTag(STATUS_BURNING);

	Duration = 3.0f;

	Period = 1.0f;

	BurningDamage = 4.0f;
}


void USActionEffect_Burning::ExecutePeriodicEffect(AActor* Instigator)
{
	Super::ExecutePeriodicEffect(Instigator);

	USGameplayFunctionLibrary::ApplyDamage(Instigator, GetOwningComponent()->GetOwner(), BurningDamage);
}
