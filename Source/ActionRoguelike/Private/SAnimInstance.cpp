// Fill out your copyright notice in the Description page of Project Settings.


#include "SAnimInstance.h"
#include "SActionComponent.h"
#include "SGameplayTags.h"

void USAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	AActor* OwningActor = GetOwningActor();
	if (OwningActor)
	{
		ActionComp = OwningActor->FindComponentByClass<USActionComponent>();
	}
}

void USAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (ActionComp)
	{
		bIsStunned = ActionComp->ActiveGameplayTags.HasTag(STATUS_STUNNED);
	}
}
