// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"
#include "AIController.h"
#include "BehaviorTree/BlackBoardComponent.h"
#include "Blueprint/UserWidget.h"
#include "BrainComponent.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "SWorldUserWidget.h"


// Sets default values
ASAICharacter::ASAICharacter()
{
	AttributeComp = CreateDefaultSubobject<USAttributeComponent>(TEXT("AttributeComp"));

	ActionComp = CreateDefaultSubobject<USActionComponent>(TEXT("ActionComp"));
	
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	//GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	
	GetMesh()->SetGenerateOverlapEvents(true);
}


void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChange.AddDynamic(this, &ASAICharacter::OnHealthChange);
	
	PawnSensingComp->OnSeePawn.AddDynamic(this, &ASAICharacter::OnSeePawn);
}


void ASAICharacter::OnHealthChange(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewValue, float Delta)
{
	if (Delta < 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("AI Health: %f"), AttributeComp->GetHealth());
		
		if (InstigatorActor != this)
		{
			SetTargetActor(InstigatorActor);
		}


		if (ActiveHealthBar == nullptr)
		{
			ActiveHealthBar = CreateWidget<USWorldUserWidget>(GetWorld(), HealthBarWidgetClass);

			if (ActiveHealthBar)
			{
				ActiveHealthBar->AttachedActor = this;
				ActiveHealthBar->AddToViewport();
			}
		}

		
		GetMesh()->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->TimeSeconds);
		
		/** Died **/
		if (NewValue <= 0.0f)
		{
			AAIController* AICont = Cast<AAIController>(GetController());
			if (AICont)
			{
				AICont->GetBrainComponent()->StopLogic("Killed.");
			}

			GetMesh()->SetCollisionProfileName("Ragdoll");
			GetMesh()->SetAllBodiesSimulatePhysics(true);
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetCharacterMovement()->DisableMovement();
			SetLifeSpan(10.0f);
		}
	}
}


AActor* ASAICharacter::GetTargetActor() const
{
	AAIController* AICont = Cast<AAIController>(GetController());

	if (AICont)
	{
		return Cast<AActor>(AICont->GetBlackboardComponent()->GetValueAsObject("TargetActor"));
	}
	
	return nullptr;
}

void ASAICharacter::SetTargetActor(AActor* NewTarget)
{
	AAIController* AICont = Cast<AAIController>(GetController());

	if (AICont)
	{
		AICont->GetBlackboardComponent()->SetValueAsObject("TargetActor", NewTarget);
	}
}


void ASAICharacter::RemoveWidget()
{
	if (PlayerSpottedWidget)
	{
		PlayerSpottedWidget->RemoveFromParent();
	}
}


void ASAICharacter::OnSeePawn(APawn* Pawn)
{
	if (GetTargetActor() != Pawn)
	{
		SetTargetActor(Pawn);
		
		MulticastOnSeePawn();
	}
	
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

void ASAICharacter::MulticastOnSeePawn_Implementation()
{
	if (PlayerSpottedWidget == nullptr)
	{
		PlayerSpottedWidget = CreateWidget<USWorldUserWidget>(GetWorld(), PlayerSpottedWidgetClass);

		if (PlayerSpottedWidget)
		{
			PlayerSpottedWidget->AttachedActor = this;
			PlayerSpottedWidget->AddToViewport(10);

			FTimerHandle TimerHandle_PlayerSpottedDelay;
			GetWorldTimerManager().SetTimer(TimerHandle_PlayerSpottedDelay, this, &ASAICharacter::RemoveWidget, 3.0f);
		}
	}
}
