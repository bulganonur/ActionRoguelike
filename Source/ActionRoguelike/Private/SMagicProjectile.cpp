// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "SAttributeComponent.h"
#include "SGameplayFunctionLibrary.h"

ASMagicProjectile::ASMagicProjectile()
{
	SphereComp->OnComponentHit.AddDynamic(this, &ASMagicProjectile::OnComponentHit);
	
	Damage = 20.0f;
}


void ASMagicProjectile::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor)
	{
		USGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, Damage, Hit);

		if (OtherActor->HasActiveCameraComponent())
		{
			UGameplayStatics::PlayWorldCameraShake(this, CamShake, Hit.ImpactPoint, 0.0f, 400.0f);
		}
		
		DrawDebugSphere
		(
			GetWorld(),
			Hit.ImpactPoint,
			400.0f,
			36,
			FColor::Blue,
			false,
			2.0f
		);
	}
}



