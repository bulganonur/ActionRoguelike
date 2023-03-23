// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"
#include "AI/SAICharacter.h"
#include "EngineUtils.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "DrawDebugHelpers.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "SPlayerState.h"


static TAutoConsoleVariable<bool> CVarSpanwBots(TEXT("su.SpawnBots"), true, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);


ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval_Bots = 2.0f;

	PickupClassArray.SetNum(2);

	CreditsPerKill = 1.0f;
}


void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	/**
	 * Continuous timer to spawn in more bots
	 * Actual amount of bots and whether it is allowed to spawn determined by the spawn logic later in the chain
	 */
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::RunQuery_SpawnBot, SpawnTimerInterval_Bots, true);
	
	
	RunQuery_SpawnPickup();
}


void ASGameModeBase::RunQuery_SpawnBot()
{
	if (!CVarSpanwBots.GetValueOnGameThread())
	{
		UE_LOG(LogTemp, Warning, TEXT("Bot spawning disabled via CVar 'CVarSpawnBots'"));
		return;
	}
	

	int32 NumOfAliveBots = 0;
	for (ASAICharacter* Bot : TActorRange<ASAICharacter>(GetWorld()))
	{
		USAttributeComponent* AIAttributeComp = USAttributeComponent::GetAttributeComp(Bot);
		if (ensure(AIAttributeComp) && AIAttributeComp->GetHealth() > 0.0f)
		{
			++NumOfAliveBots;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Number of alive bots: %i"), NumOfAliveBots);


	float MaxBotCount = 10.0f;
	if (DifficultyCurve)
	{
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}


	if (NumOfAliveBots >= MaxBotCount)
	{
		UE_LOG(LogTemp, Warning, TEXT("Max number of bots present! Skipping bot spawn"));
		return;
	}
	

	FEnvQueryRequest Request(SpawnBotQuery, this);
	Request.Execute(EEnvQueryRunMode::RandomBest5Pct, this, &ASGameModeBase::OnQueryFinished_Bot);
}


void ASGameModeBase::RunQuery_SpawnPickup()
{
	FEnvQueryRequest Request(SpawnPickupQuery, this);
	Request.Execute(EEnvQueryRunMode::AllMatching, this, &ASGameModeBase::OnQueryFinished_Pickup);
}


void ASGameModeBase::OnQueryFinished_Bot(TSharedPtr<FEnvQueryResult> Result)
{
	FEnvQueryResult* QueryResult = Result.Get();

	if (!QueryResult->IsSuccsessful())
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn Bot EQS Query Failed!"));
		return;
	}


	TArray<FVector> QueryLocations;
	QueryResult->GetAllAsLocations(QueryLocations);

	if (QueryLocations.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("EQS Resulted with no Actors/Vectors, 0 (zero) input for Spawn bot"));
		return;
	}


	GetWorld()->SpawnActor<AActor>(MinionClass, QueryLocations[0], FRotator::ZeroRotator);


	DrawDebugBox
	(
		GetWorld(),
		QueryLocations[0],
		FVector(400.0f),
		FColor::Yellow,
		false,
		10.0f,
		(uint8)0U,
		20.0f
	);
}


void ASGameModeBase::OnQueryFinished_Pickup(TSharedPtr<FEnvQueryResult> Result)
{
	FEnvQueryResult* QueryResult = Result.Get();
	
	if (!QueryResult->IsSuccsessful())
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn Pickups EQS Query Failed!"));
		return;
	}


	TArray<FVector> QueryLocations;
	QueryResult->GetAllAsLocations(QueryLocations);
	

	for (FVector QLocation : QueryLocations)
	{
		UE_LOG(LogTemp, Log, TEXT("QL: %s"), *QLocation.ToString());
	}

	if (QueryLocations.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("EQS Resulted with no Actors/Vectors, 0 (zero) input for Spawn Pickup"));
		return;
	}

	for (FVector QLocation : QueryLocations)
	{
		int32 RndPickup = FMath::RandRange(0, 1);
		
		GetWorld()->SpawnActor<AActor>(PickupClassArray[RndPickup], QLocation, FRotator::ZeroRotator);

		DrawDebugBox
		(
			GetWorld(),
			QLocation,
			FVector(200.0f),
			FColor::Purple,
			false,
			10.0f,
			(uint8)0U,
			20.0f
		);
	}
}


void ASGameModeBase::KillAll()
{
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;

		USAttributeComponent* AIAttributeComp = USAttributeComponent::GetAttributeComp(Bot);
		if (ensure(AIAttributeComp) && AIAttributeComp->GetHealth() > 0.0f)
		{
			AIAttributeComp->Kill(this); // @fix: pass in character as the argument for in-game credits?
		}
	}
}


void ASGameModeBase::OnActorKilled(AActor* Victim, AActor* Killer)
{
	ASCharacter* PlayerChar = Cast<ASCharacter>(Victim);
	if (PlayerChar)
	{
		FTimerHandle TimerHandle_RespawnDelay;
		FTimerDelegate Delegate;

		Delegate.BindUFunction(this, "RespawnPlayerElapsed", PlayerChar->GetController());

		float RespawnDelay = 2.0f;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, false);
	}

	PlayerChar = Cast<ASCharacter>(Killer);
	if (PlayerChar)
	{
		ASPlayerState* PlayerState = PlayerChar->GetPlayerState<ASPlayerState>();
		if (PlayerState)
		{
			PlayerState->SetCredits(CreditsPerKill);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("OnActorKilled! Victim: %s, Killer: %s"), *GetNameSafe(Victim), *GetNameSafe(Killer));
}


void ASGameModeBase::RespawnPlayerElapsed(AController* Controller)
{
	if (ensure(Controller))
	{
		Controller->UnPossess();

		RestartPlayer(Controller);
	}
}