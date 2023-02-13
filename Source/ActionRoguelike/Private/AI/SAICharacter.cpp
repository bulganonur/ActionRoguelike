// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"
#include "AIController.h"
#include "BehaviorTree/BlackBoardComponent.h"
#include "DrawDebugHelpers.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
ASAICharacter::ASAICharacter()
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComp->OnSeePawn.AddDynamic(this, &ASAICharacter::OnSeePawn);
}

void ASAICharacter::OnSeePawn(APawn* Pawn)
{
	AAIController* AICont = Cast<AAIController>(GetController());
	
	if (AICont)
	{
		AICont->GetBlackboardComponent()->SetValueAsObject("TargetActor", Pawn);

		DrawDebugString
		(
			GetWorld(),
			GetActorLocation(),
			"PLAYERSPOTTED !!!",
			nullptr,
			FColor::White,
			2.0f,
			true
		);
	}
}






