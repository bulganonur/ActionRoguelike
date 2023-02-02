// Fill out your copyright notice in the Description page of Project Settings.


#include "SPickup.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ASPickup::ASPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	RootComponent = CapsuleComp;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ASPickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

