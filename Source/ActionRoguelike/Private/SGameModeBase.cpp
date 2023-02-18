// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"
#include "AI/SAICharacter.h"
#include "EngineUtils.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "SAttributeComponent.h"

ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval = 2.0f;

}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	/**
	 * Continuous timer to spawn in more bots
	 * Actual amount of bots and whether it is allowed to spawn determined by the spawn logic later in the chain
	 */
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);

}

void ASGameModeBase::SpawnBotTimerElapsed()
{
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);

	if (ensure(QueryInstance))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnQueryFinished);
	}
}

void ASGameModeBase::OnQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn Bot EQS Query Failed!"));
		return;
	}


	int32 NumOfAliveBots = 0;
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;

		USAttributeComponent* AIAttributeComp = Cast<USAttributeComponent>(Bot->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (AIAttributeComp && AIAttributeComp->GetHealth() > 0.0f)
		{
			++NumOfAliveBots;
		}
	}


	float MaxBotCount = 10.0f;
	if (DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}
	

	if (NumOfAliveBots >= MaxBotCount)
	{
		UE_LOG(LogTemp, Warning, TEXT("Max number of bots spawned!"));
		return;
	}


	TArray<FVector> QueryLocations = QueryInstance->GetResultsAsLocations();

	if (QueryLocations.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("EQS Resulted with no Actors/Vectors, 0 (zero) input for Spawn bot"));
		return;
	}

	GetWorld()->SpawnActor<AActor>(MinionClass, QueryLocations[0], FRotator::ZeroRotator);
}
