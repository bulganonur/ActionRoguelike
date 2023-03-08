// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_Heal.h"
#include "AI/SAICharacter.h"
#include "AIController.h"
#include "SAttributeComponent.h"

EBTNodeResult::Type USBTTask_Heal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* AICont = OwnerComp.GetAIOwner();
	if (AICont)
	{
		ASAICharacter* AIChar = Cast<ASAICharacter>(AICont->GetPawn());
		if (AIChar)
		{
			USAttributeComponent* AIAttributeComp = USAttributeComponent::GetAttributeComp(AIChar);
			if (AIAttributeComp)
			{
				AIAttributeComp->ApplyHealthChange(AIChar, AIAttributeComp->GetHealthMax());

				return EBTNodeResult::Succeeded;
			}
		}
	}
	
	return EBTNodeResult::Failed;
}
