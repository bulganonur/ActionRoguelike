// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameModeBase.h"
#include "SGameModeBase.generated.h"

class UCurveFloat;
class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;


UCLASS()
class ACTIONROGUELIKE_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	ASGameModeBase();

	virtual void StartPlay() override;

	UFUNCTION(Exec)
	void KillAll();

	virtual void OnActorKilled(AActor* Victim, AActor* Killer);

protected:

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AActor> MinionClass;
	
	/** Used TArray Instead, leaving commented for consideration **/
	/*UPROPERTY(EditDefaultsOnly, Category = "Pickups")
	TSubclassOf<AActor> HealthPotionClass;

	UPROPERTY(EditDefaultsOnly, Category = "Pickups")
	TSubclassOf<AActor> CoinClass;*/
	
	UPROPERTY(EditDefaultsOnly, Category = "Pickups")
	TArray<TSubclassOf<AActor>> PickupClassArray;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly, Category = "Pickups")
	UEnvQuery* SpawnPickupQuery;
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* DifficultyCurve;

	FTimerHandle TimerHandle_SpawnBots;
	FTimerHandle TimerHandle_SpawnPickups;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnTimerInterval_Bots;

	void SpawnBotTimerElapsed();
	void SpawnPickup();

	void OnQueryFinished_Bot(TSharedPtr<FEnvQueryResult> Result);
	void OnQueryFinished_Pickup(TSharedPtr<FEnvQueryResult> Result);

	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);
};
