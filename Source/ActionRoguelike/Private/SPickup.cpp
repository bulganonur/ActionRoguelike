// Fill out your copyright notice in the Description page of Project Settings.


#include "SPickup.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ASPickup::ASPickup()
{
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	RootComponent = CapsuleComp;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SetReplicates(true);
}

void ASPickup::Reactivate()
{
	StaticMesh->SetVisibility(true);
}

void ASPickup::Deactivate()
{
	StaticMesh->SetVisibility(false);
}
