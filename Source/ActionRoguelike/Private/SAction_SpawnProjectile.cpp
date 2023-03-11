// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction_SpawnProjectile.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

USAction_SpawnProjectile::USAction_SpawnProjectile()
{
	ActionAnimDelay = 0.2f;
	RHandMuzzleSocket = "Muzzle_01";
}

void USAction_SpawnProjectile::StartAction(AActor* Instigator)
{
	Super::StartAction(Instigator);

	ACharacter* Character = Cast<ACharacter>(Instigator);
	if (Character)
	{
		Character->PlayAnimMontage(ActionAnim);
		
		
		FTimerHandle TimerHandle_Action;
		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "SpawnProjectile", Character);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_Action, Delegate, ActionAnimDelay, false);
	}
}

void USAction_SpawnProjectile::SpawnProjectile(ACharacter* InstigatorChar)
{
	UGameplayStatics::SpawnEmitterAttached(VFX_MuzzleFlash, InstigatorChar->GetMesh(), RHandMuzzleSocket);

	if (ensureAlways(ProjectileClass))
	{

		/* Variables needed for Sweep */

		FCollisionShape Shape;
		Shape.SetSphere(20.0f);

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(InstigatorChar); // ignore player

		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);

		FVector TraceStart = InstigatorChar->GetPawnViewLocation();
		FVector TraceEnd = TraceStart + (InstigatorChar->GetControlRotation().Vector() * 5000);

		FHitResult Hit;

		GetWorld()->SweepSingleByObjectType(Hit, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params); // Sweep

		if (Hit.bBlockingHit)
		{
			TraceEnd = Hit.ImpactPoint; // override TraceEnd with impact point in world
		}

		/* Variables needed for SpawnActor */

		FVector HandLocation = InstigatorChar->GetMesh()->GetSocketLocation(RHandMuzzleSocket);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = InstigatorChar;

		FRotator SpawnRotation = FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();

		FTransform SpawnTM = FTransform(SpawnRotation, HandLocation);

		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams); // Spawn

		DrawDebugLine
		(
			GetWorld(),
			TraceStart,
			TraceEnd,
			FColor::Red,
			true,
			0.0f,
			0,
			2.5f
		);

		DrawDebugLine
		(
			GetWorld(),
			HandLocation,
			TraceEnd,
			FColor::Blue,
			true,
			0.0f,
			0,
			2.5f
		);

	}

	StopAction(InstigatorChar);
}
