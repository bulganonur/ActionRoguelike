// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SSaveGame.generated.h"


USTRUCT()
struct FActorSaveData
{
	GENERATED_BODY()

public:

	/** Identifier for which actor this belongs to */
	UPROPERTY()
	FName ActorName;

	/** For movable actors */
	UPROPERTY()
	FTransform ActorTransform;

	/** Filled with binary data */
	UPROPERTY()
	TArray<uint8> ByteData;
};


UCLASS()
class ACTIONROGUELIKE_API USSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY()
	float Credits;

	UPROPERTY()
	TArray<FActorSaveData> SavedActors;
	
};
