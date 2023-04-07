// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGameplayInterface.h"
#include "SItemChest.generated.h"


class UStaticMeshComponent;


UCLASS()
class ACTIONROGUELIKE_API ASItemChest : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()

	
public:	
	
	ASItemChest();

	void Interact_Implementation(APawn* InstigatorPawn);

	void OnActorLoaded_Implementation();
	
	UPROPERTY(EditAnywhere)
	float TargetPitch;

	UPROPERTY(ReplicatedUsing = "OnRep_IsLidOpen", BlueprintReadOnly, SaveGame) // RepNotify
	bool bIsLidOpen;

	UFUNCTION()
	void OnRep_IsLidOpen();

protected:

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LidMesh;
};
