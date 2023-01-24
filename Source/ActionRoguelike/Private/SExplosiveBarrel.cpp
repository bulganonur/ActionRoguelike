// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	RootComponent = StaticMesh;
	StaticMesh->SetSimulatePhysics(true);

	RadialForce = CreateDefaultSubobject<URadialForceComponent>("RadialForce");
	RadialForce->SetupAttachment(StaticMesh);

	RadialForce->ImpulseStrength = 3500.0f;
	RadialForce->Radius = 500.0f;
	RadialForce->bImpulseVelChange = true;
	RadialForce->bAutoActivate = false;
	RadialForce->AddCollisionChannelToAffect(ECollisionChannel::ECC_WorldDynamic);
	
}


void ASExplosiveBarrel::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	StaticMesh->OnComponentHit.AddDynamic(this, &ASExplosiveBarrel::OnComponentHit);
}


// Called when the game starts or when spawned
void ASExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void ASExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ASExplosiveBarrel::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	
	FString DebugText = FString::Printf(TEXT("Impact Point: %s"), *Hit.ImpactPoint.ToString());
	
	DrawDebugString
	(
		GetWorld(),
		Hit.ImpactPoint,
		DebugText,
		nullptr,
		FColor::Green,
		1.0f,
		true
	);
	
	APawn* TestPawn = Cast<APawn>(OtherActor);
	
	if (!TestPawn)
	{
		RadialForce->FireImpulse();
	}

	TestPawn = nullptr;
}

