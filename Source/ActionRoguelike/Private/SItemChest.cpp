// Fill out your copyright notice in the Description page of Project Settings.


#include "SItemChest.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ASItemChest::ASItemChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	LidMesh->SetupAttachment(BaseMesh);

	TargetPitch = 110.0f;

	SetReplicates(true);
}


void ASItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	bIsLidOpen = !bIsLidOpen;
	OnRep_IsLidOpen();
}


void ASItemChest::OnRep_IsLidOpen()
{
	float CurrentPitch = bIsLidOpen ? TargetPitch : 0.0f;
	LidMesh->SetRelativeRotation(FRotator(CurrentPitch, 0.0f, 0.0f));
}


void ASItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASItemChest, bIsLidOpen);
}