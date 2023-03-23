// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SActionComponent.h"
#include "SActionEffect.h"
#include "SAttributeComponent.h"
#include "SGameplayFunctionLibrary.h"

ASMagicProjectile::ASMagicProjectile()
{
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnComponentBeginOverlap);
	
	Damage = 20.0f;
}


void ASMagicProjectile::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != GetInstigator())
	{
		USActionComponent* ActionComp = Cast<USActionComponent>(OtherActor->GetComponentByClass(USActionComponent::StaticClass()));
		if (ActionComp && ActionComp->ActiveGameplayTags.HasTag(FGameplayTag::RequestGameplayTag("Status.Parrying")))
		{
			MovementComp->Velocity = -(MovementComp->Velocity);
			SetInstigator(Cast<APawn>(OtherActor));
			return;
		}

		USGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, Damage, SweepResult);

		if (ActionComp)
		{
			ActionComp->AddAction(GetInstigator(), BurningEffectClass);
		}
		
		if (OtherActor->HasActiveCameraComponent())
		{
			UGameplayStatics::PlayWorldCameraShake(this, CamShake, SweepResult.ImpactPoint, 0.0f, 400.0f);
		}

		Explode();


		DrawDebugSphere
		(
			GetWorld(),
			SweepResult.ImpactPoint,
			400.0f,
			36,
			FColor::Blue,
			false,
			2.0f
		);
	}
}



