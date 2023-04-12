// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "Engine/DataTable.h"
#include "GameFramework/GameModeBase.h"
#include "SGameModeBase.generated.h"

class UCurveFloat;
class UDataTable;
class UEnvQuery;
class USMonsterData;
class USSaveGame;


/** DataTable Row for spawning monsters in GameMode */
USTRUCT(BlueprintType)
struct FMonsterInfoRow : public FTableRowBase
{
	GENERATED_BODY()

public:

	FMonsterInfoRow()
	{
		Weight = 1.0f;
		SpawnCost = 5.0f;
		KillReward = 1.0f;
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FPrimaryAssetId MonsterID;

	/** Relative chance to pick this monster */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Weight;

	/** Points required by GameMode to spawn this unit */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float SpawnCost;

	/** Amount of Credits awarded to killer of this unit */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float KillReward;
};


UCLASS()
class ACTIONROGUELIKE_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	ASGameModeBase();

	void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	void StartPlay() override;

	void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	UFUNCTION(Exec)
	void KillAll();

	virtual void OnActorKilled(AActor* Victim, AActor* Killer);

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void WriteSaveGame();

	void LoadSaveGame();

protected:

	UPROPERTY()
	USSaveGame* CurrentSaveGame;

	FString SlotName;

	/** All available monsters */
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UDataTable* MonsterTable;

	/** Using DataTable instead */
	/*UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AActor> MinionClass;*/
	
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

	void RunQuery_SpawnBot();
	void RunQuery_SpawnPickup();

	void OnQueryFinished_Bot(TSharedPtr<FEnvQueryResult> Result);
	void OnQueryFinished_Pickup(TSharedPtr<FEnvQueryResult> Result);

	void OnMonsterLoad(FPrimaryAssetId MonsterID, FVector SpawnVector);

	UFUNCTION()
	void RespawnPlayerElapsed(AController* Controller);

	float CreditsPerKill;
};
