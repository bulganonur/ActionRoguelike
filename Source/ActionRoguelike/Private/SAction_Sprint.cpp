// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_Sprint.h"
#include "GameFramework/CharacterMovementComponent.h"


USAction_Sprint::USAction_Sprint()
{
	SprintBonus = 2000.0f;
	ActionName = "Sprint";

}


void USAction_Sprint::StartAction(AActor* Instigator)
{
	Super::StartAction(Instigator);

	UCharacterMovementComponent* MoveComp = Cast<UCharacterMovementComponent>(Instigator->GetComponentByClass(UCharacterMovementComponent::StaticClass()));
	if (MoveComp)
	{
		MoveComp->MaxWalkSpeed += SprintBonus;
	}
}


void USAction_Sprint::StopAction(AActor* Instigator)
{
	Super::StopAction(Instigator);

	UCharacterMovementComponent* MoveComp = Cast<UCharacterMovementComponent>(Instigator->GetComponentByClass(UCharacterMovementComponent::StaticClass()));
	if (MoveComp)
	{
		MoveComp->MaxWalkSpeed -= SprintBonus;
	}
}


