// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGameplayInterface.h"
#include "SPickup.generated.h"

class UCapsuleComponent;
class UStaticMeshComponent;


UCLASS()
class ACTIONROGUELIKE_API ASPickup : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPickup();

	FText GetInteractText_Implementation(APawn* InstigatorPawn);

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UCapsuleComponent* CapsuleComp;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UStaticMeshComponent* StaticMesh;

	FTimerHandle TimerHandle_HideAndCooldown;

	UPROPERTY(EditDefaultsOnly, Category = "Pickup Attributes")
	float ReactivateDelay;

	UPROPERTY(ReplicatedUsing = "OnRep_IsActive")
	bool bIsActive;

	UFUNCTION()
	void OnRep_IsActive();

	void Reactivate();
	void Deactivate();

	void SetPickupState(bool bNewIsActive);

	void HideAndCooldown();
};
