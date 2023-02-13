// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_RangedAttack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type USBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	AAIController* AICont = OwnerComp.GetAIOwner();
	if (ensure(AICont))
	{
		ACharacter* AIChar = Cast<ACharacter>(AICont->GetPawn());
		if (AIChar == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		FVector AIMuzzleLocation = AIChar->GetMesh()->GetSocketLocation("Muzzle_Front");

		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
		if (TargetActor == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		
		FRotator SpawnRotation = FRotationMatrix::MakeFromX(TargetActor->GetActorLocation() - AIMuzzleLocation).Rotator();

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = AIChar;

		AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ProjectileClass, AIMuzzleLocation, SpawnRotation, SpawnParams);

		return SpawnedActor ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}
	
	
	return EBTNodeResult::Failed;
}
