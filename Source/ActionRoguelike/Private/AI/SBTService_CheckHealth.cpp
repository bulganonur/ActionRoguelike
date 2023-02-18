// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckHealth.h"
#include "AIController.h"
#include "AI/SAICharacter.h"
#include "BehaviorTree/BlackBoardComponent.h"
#include "SAttributeComponent.h"

void USBTService_CheckHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AICont = OwnerComp.GetAIOwner();
	UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
	if (AICont && BlackBoardComp)
	{
		ASAICharacter* AIChar = Cast<ASAICharacter>(AICont->GetPawn());
		if (AIChar)
		{
			USAttributeComponent* AIAttributeComp = Cast<USAttributeComponent>(AIChar->GetComponentByClass(USAttributeComponent::StaticClass()));
			if (AIAttributeComp)
			{
				bool bAboveHealthThreshold = AIAttributeComp->GetHealth() > 80.0f;
				BlackBoardComp->SetValueAsBool(HealthThresholdKey.SelectedKeyName, bAboveHealthThreshold);
			}
		}
	}
}
