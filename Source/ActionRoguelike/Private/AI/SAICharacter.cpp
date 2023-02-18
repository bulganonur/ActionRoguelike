// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"
#include "AIController.h"
#include "BehaviorTree/BlackBoardComponent.h"
#include "DrawDebugHelpers.h"
#include "Perception/PawnSensingComponent.h"
#include "SAttributeComponent.h"

// Sets default values
ASAICharacter::ASAICharacter()
{
	AttributeComp = CreateDefaultSubobject<USAttributeComponent>(TEXT("AttributeComp"));
	
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);
	
	PawnSensingComp->OnSeePawn.AddDynamic(this, &ASAICharacter::OnSeePawn);
}


void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if (Delta < 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("AI Health: %f"), AttributeComp->GetHealth());
		
		/*if (NewHealth <= 0.0f)
		{
			TurnOff();
		}*/
	}
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






