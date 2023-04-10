// Fill out your copyright notice in the Description page of Project Settings.


#include "SPickup.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASPickup::ASPickup()
{
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	RootComponent = CapsuleComp;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ReactivateDelay = 2.0f;

	bIsActive = true;

	bReplicates = true;
}

FText ASPickup::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	return FText::GetEmpty();
}

void ASPickup::Reactivate()
{
	SetPickupState(true);
}

void ASPickup::Deactivate()
{
	SetPickupState(false);
}

void ASPickup::HideAndCooldown()
{
	SetPickupState(false);

	GetWorldTimerManager().SetTimer(TimerHandle_HideAndCooldown, this, &ASPickup::Reactivate, ReactivateDelay);
}

void ASPickup::SetPickupState(bool bNewIsActive)
{
	bIsActive = bNewIsActive;
	OnRep_IsActive();
}

void ASPickup::OnRep_IsActive()
{
	SetActorEnableCollision(bIsActive);

	/** Set visibility on root and all children */
	RootComponent->SetVisibility(bIsActive, true);
}

void ASPickup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPickup, bIsActive);
}
