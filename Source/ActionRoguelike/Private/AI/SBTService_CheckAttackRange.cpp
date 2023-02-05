// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckAttackRange.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void USBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackBoardComp))
	{
		AActor* TargetActor = Cast<AActor>(BlackBoardComp->GetValueAsObject("TargetActor"));
		if (TargetActor)
		{
			AAIController* AICont = OwnerComp.GetAIOwner();
			if (ensure(AICont))
			{
				APawn* AIPawn = AICont->GetPawn();

				if (ensure(AIPawn))
				{
					float Distance = FVector::Dist(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());

					bool bIsInRange = Distance < 2000.0f;
					bool bHasLOS = false;
					
					if (bIsInRange)
					{
						bHasLOS = AICont->LineOfSightTo(TargetActor);
					}

					BlackBoardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, (bIsInRange && bHasLOS));
				}
			}
			
		}
	}
}
