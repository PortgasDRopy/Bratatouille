// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Blueprint/UserWidget.h"
#include "UMG.h"
#include "RGameState.generated.h"

class AREnemy;
class ASpawner;

USTRUCT()
struct FWave
{
	GENERATED_BODY()
	FWave()
	{
		Level = 0;
	}
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AREnemy>> EnemyList;

	UPROPERTY(EditAnywhere)
	int Level;
};

/**
 *
 */
UCLASS()
class BRATATOUILLE_API ARGameState : public AGameStateBase
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void PlaySecondLevelSequence();

private:
	void LoadLevelUI();

	UPROPERTY()
	UUserWidget* CurrentUI;
public:
	ARGameState();
	//------------------------------------------------------------
	//-UI
	void UpdateUI();
	void SetMouseForMenu();
	void SetMouseForGame();
	UFUNCTION(BlueprintCallable)
	static void SetMouseForGame(UObject* WorldContextObject);
	static ARGameState* GetGameState(UObject* WorldContextObject);
	static void AddExp(UObject* WorldContextObject, int Amount);
	static void AddCurrency(UObject* WorldContextObject, int Amount);
	static void SetMaxHp(UObject* WorldContextObject, int Amount);
	static void SetHp(UObject* WorldContextObject, int Amount);
	void LevelUP();
	UFUNCTION(BlueprintCallable)
	void OpenShop();
	static void OpenMenu(UObject* WorldContextObject);
	static void Death(UObject* WorldContextObject);
	UFUNCTION(BlueprintCallable)
	void BuySword();
	UFUNCTION(BlueprintCallable)
	void BuyGun();
	//--------------------
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TMap<FName, TSubclassOf<UUserWidget>> LevelUIMap;
	UPROPERTY(EditDefaultsOnly, Category = "LevelUP")
	TSubclassOf<UUserWidget> LevelUPUI;
	UPROPERTY(EditDefaultsOnly, Category = "Shop")
	TSubclassOf<UUserWidget> ShopUI;
	UPROPERTY(EditDefaultsOnly, Category = "Death")
	TSubclassOf<UUserWidget> DeathUI;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Settings")
	TSubclassOf<UUserWidget> SettingsUI;
	//--------------------
	int mHp;
	int mMaxHp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int mCurrency;
	int mExp;
	int mMaxExp;
	int mLevel;
	int mWave;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int mLevelsGottenDuringWave;
	UPROPERTY(BlueprintReadWrite)
	bool isPaused;
	UPROPERTY(BlueprintReadWrite, Category = "Audio")
	UAudioComponent* MusicAudioComponent;
	bool isCinematic;
	//------------------------------------------------------------

	//------------------------------------------------------------
	//Spawner
public:
	static void RemoveFromLifeEnemyList(UObject* WorldContextObject, AREnemy* enemy);
private:
	void Spawn();
	void SpawnDelay();

	UPROPERTY(EditAnywhere, Category = "Spawner Props")
	TArray<FWave> EnemyWaves;
	TArray<AREnemy*> mLifeEnemy;
	ASpawner* SpawnPoints;

	bool mCanSpawn = true;
	int mCurrSpawnId = 0;
	int mCurrWave = 0;
	int mSpawnDelay = 1;


	//-------------------------------------------------------------
};