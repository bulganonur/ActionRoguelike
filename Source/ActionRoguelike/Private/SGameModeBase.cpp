// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"
#include "AI/SAICharacter.h"
#include "DrawDebugHelpers.h"
#include "EngineUtils.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "SGameplayInterface.h"
#include "SPlayerState.h"
#include "SSaveGame.h"


static TAutoConsoleVariable<bool> CVarSpanwBots(TEXT("su.SpawnBots"), true, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);


ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval_Bots = 2.0f;

	PickupClassArray.SetNum(2);

	CreditsPerKill = 1.0f;

	SlotName = "SaveGame00";
}


void ASGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	LoadSaveGame();
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


void ASGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	ASPlayerState* PS = NewPlayer->GetPlayerState<ASPlayerState>();
	if (PS)
	{
		PS->LoadPlayerState(CurrentSaveGame);
	}
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


void ASGameModeBase::WriteSaveGame()
{
	/** Iterate all player states, we don't hve proper ID to match yet (requires Steam or EOS) */
	for (int32 i = 0; i < GameState->PlayerArray.Num(); ++i)
	{
		ASPlayerState* PS = Cast<ASPlayerState>(GameState->PlayerArray[i]);
		if (PS)
		{
			PS->SavePlayerState(CurrentSaveGame);
			break; // Single player only at this point
		}
	}


	CurrentSaveGame->SavedActors.Empty();


	/** Iterate the entire world of actors */
	for (AActor* Actor : TActorRange<AActor>(GetWorld()))
	{
		/** Only interested in our 'gameplay actors' */
		if (!Actor->Implements<USGameplayInterface>())
		{
			continue;
		}

		FActorSaveData ActorData;
		ActorData.ActorName = Actor->GetFName();
		ActorData.ActorTransform = Actor->GetTransform();

		/** Pass the ByteData array to fill data from actor */
		FMemoryWriter MemWriter(ActorData.ByteData);

		FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);

		/** Find only variables with UPROPERTY(SaveGame) */
		Ar.ArIsSaveGame = true;

		/** Converts Actor's UPROPERTY(SaveGame)s into binary array - Serialization */
		Actor->Serialize(Ar);

		CurrentSaveGame->SavedActors.Add(ActorData);
	}

	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
}


void ASGameModeBase::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		if (CurrentSaveGame == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load SaveGame data!"));
			return;
		}

		UE_LOG(LogTemp, Log, TEXT("Loaded SaveGame data!"));

		/** Iterate the entire world of actors */
		for (AActor* Actor : TActorRange<AActor>(GetWorld()))
		{
			/** Only interested in our 'gameplay actors' */
			if (!Actor->Implements<USGameplayInterface>())
			{
				continue;
			}

			for (const FActorSaveData& ActorData : CurrentSaveGame->SavedActors)
			{
				if (ActorData.ActorName == Actor->GetFName())
				{
					Actor->SetActorTransform(ActorData.ActorTransform);

					/** Read what's written by WriteSaveGame() */
					FMemoryReader MemReader(ActorData.ByteData);

					FObjectAndNameAsStringProxyArchive Ar(MemReader, true);

					Ar.ArIsSaveGame = true;

					/** Converts binary array back into Actor's variables - Deserialization */
					Actor->Serialize(Ar);

					ISGameplayInterface::Execute_OnActorLoaded(Actor);

					break;
				}
			}
		}
	}
	else
	{
		CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::CreateSaveGameObject(USSaveGame::StaticClass()));

		UE_LOG(LogTemp, Log, TEXT("Created new SaveGame data!"));
	}
}