// Shoot Them Up Game, All Righs Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "STUCoreTypes.h"
#include "STUGameModeBase.generated.h"

class AAIController;

UCLASS()
class SHOOTTHEMUP_API ASTUGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ASTUGameModeBase();

	virtual void StartPlay() override;
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
		TSubclassOf<AAIController> AIControllerClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
		TSubclassOf<APawn> AIPawnClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game")
		FGameData GameData;

private:
	int32 CurrentRound = 1;
	int32 RoundCountDown = 0;
	FTimerHandle GameRoundTimerHandle;

	void SpawnBots();
	void StartRound();
	void GameTimerUpdate();

	void ResetPlayers();
	void ResetOnePlayer(AController* Controller);
};