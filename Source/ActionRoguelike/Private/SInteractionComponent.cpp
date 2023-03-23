// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "DrawDebugHelpers.h"
#include "SGameplayInterface.h"
#include "SWorldUserWidget.h"


USInteractionComponent::USInteractionComponent()
{
	CollisionChannel = ECC_WorldDynamic;
}


void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	
	
}


void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn->IsLocallyControlled())
	{
		FindBestInteractable();
	}
}


void USInteractionComponent::FindBestInteractable()
{
	AActor* MyOwner = GetOwner();
	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End = EyeLocation + (EyeRotation.Vector() * 1000);

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

	TArray<FHitResult> Hits;

	FCollisionShape Shape;
	float Radius = 35.0f;
	Shape.SetSphere(Radius);

	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape);

	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;

	/** Clear ref before trying to fill */
	FocusedActor = nullptr;

	for (FHitResult& Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();

		if (HitActor)
		{
			if (HitActor->Implements<USGameplayInterface>())
			{
				FocusedActor = HitActor;
			}
		}
		break;
	}

	if (FocusedActor)
	{
		if (ensure(DefaultWidgetClass) && DefaultWidgetInstance == nullptr)
		{
			DefaultWidgetInstance = CreateWidget<USWorldUserWidget>(GetWorld(), DefaultWidgetClass);
		}

		if (DefaultWidgetClass)
		{
			DefaultWidgetInstance->AttachedActor = FocusedActor;
			if (!DefaultWidgetInstance->IsInViewport())
			{
				DefaultWidgetInstance->AddToViewport();
			}

		}
	}
	else
	{
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->RemoveFromParent();
		}
	}
}


void USInteractionComponent::PrimaryInteraction()
{
	ServerInteract(FocusedActor);
}


void USInteractionComponent::ServerInteract_Implementation(AActor* InFocus)
{
	if (InFocus == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "No focus actor to interact");
		return;
	}

	APawn* MyPawn = Cast<APawn>(GetOwner());

	ISGameplayInterface::Execute_Interact(InFocus, MyPawn);
}