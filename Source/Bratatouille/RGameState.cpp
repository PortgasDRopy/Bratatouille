// Fill out your copyright notice in the Description page of Project Settings.


#include "RGameState.h"
#include "Spawner.h"
#include "REnemy.h"
#include "BratatouilleCharacter.h"
#include "RSword.h"
#include "RGun.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"
#include <Kismet/GameplayStatics.h>

ARGameState::ARGameState()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ARGameState::BeginPlay()
{
    Super::BeginPlay();



    LoadLevelUI();
    FName LevelName = *UGameplayStatics::GetCurrentLevelName(this);
    if (LevelName == "ThirdPersonMap") {
        SetMouseForGame();
        mWave = 0;
        mCurrency = 0;
        mExp = 0;
        mMaxExp = 100;
        mMaxHp = 100;
        mHp = mMaxHp;
        TArray<AActor*> FoundActors;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawner::StaticClass(), FoundActors);
        SpawnPoints = Cast<ASpawner>(FoundActors[0]);
        UpdateUI();
        LevelUP();
        //OpenShop();
        mCanSpawn = true;
        mLevelsGottenDuringWave = 0;
        isPaused = false;
        isCinematic = false;
    }
    else {
        SetMouseForMenu();
        mCanSpawn = false;
    }
}

void ARGameState::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    FName LevelName = *UGameplayStatics::GetCurrentLevelName(this);
    if (LevelName == "ThirdPersonMap") {
        if (EnemyWaves.Num() == 0)
            return;

        if (mCanSpawn && mCurrSpawnId < EnemyWaves[mCurrWave].EnemyList.Num())
        {
            Spawn();
        }

        else if (mLifeEnemy.Num() == 0 && mCurrWave + 1 <= EnemyWaves.Num() - 1 && mCurrSpawnId == EnemyWaves[mCurrWave].EnemyList.Num())
        {
            mCurrWave++;
            mWave++;
            mCurrSpawnId = 0;
            UpdateUI();
            TArray<AActor*> FoundActors;
            UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARBall::StaticClass(), FoundActors);
            for (int i = 0; i < FoundActors.Num(); i++) {
                FoundActors[i]->Destroy();
            }
            AActor* player = UGameplayStatics::GetPlayerPawn(this, 0);
            player->SetActorRotation(FRotator(0.f, 0.f, 0.f));
            LevelUP();
        }
        else if (mCurrWave + 1 == EnemyWaves.Num() && isCinematic == false && mLifeEnemy.Num() == 0) {
            isCinematic = true;
            if (MusicAudioComponent && MusicAudioComponent->IsPlaying())
            {
                MusicAudioComponent->Stop();
            }
            AActor* player = UGameplayStatics::GetPlayerPawn(this, 0);
            ABratatouilleCharacter* character = Cast<ABratatouilleCharacter>(player);
            if (character) {
                character->SetActorLocation(FVector(-4270, 8440, 2700));
            }
            CurrentUI->RemoveFromParent();
            TArray<AActor*> FoundActors;
            UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARBall::StaticClass(), FoundActors);
            for (int i = 0; i < FoundActors.Num(); i++) {
                FoundActors[i]->Destroy();
            }
            TArray<AActor*> FoundActors2;
            UGameplayStatics::GetAllActorsOfClass(GetWorld(), AREnemy::StaticClass(), FoundActors2);
            for (int i = 0; i < FoundActors2.Num(); i++) {
                FoundActors2[i]->Destroy();
            }
            ULevelSequence* LevelSequence1 = LoadObject<ULevelSequence>(nullptr, TEXT("/Game/Cinematics/NewLevelSequence.NewLevelSequence"));

            if (LevelSequence1)
            {
                // Spawn the Level Sequence Actor
                ALevelSequenceActor* LevelSequenceActor1 = GetWorld()->SpawnActor<ALevelSequenceActor>(ALevelSequenceActor::StaticClass());

                if (LevelSequenceActor1)
                {
                    LevelSequenceActor1->SetSequence(LevelSequence1);

                    // Play the first sequence
                    ULevelSequencePlayer* SequencePlayer1 = LevelSequenceActor1->GetSequencePlayer();
                    SequencePlayer1->Play();

                    // Bind the event to play the second sequence when the first finishes
                    SequencePlayer1->OnFinished.AddDynamic(this, &ARGameState::PlaySecondLevelSequence);
                }
            }
        }
    }
}

void ARGameState::PlaySecondLevelSequence()
{
    // Play second Level Sequence
    ULevelSequence* LevelSequence2 = LoadObject<ULevelSequence>(nullptr, TEXT("/Game/Cinematics/cinematic2/NewLevelSequence2.NewLevelSequence2"));

    if (LevelSequence2)
    {
        // Spawn the Level Sequence Actor for the second sequence
        ALevelSequenceActor* LevelSequenceActor2 = GetWorld()->SpawnActor<ALevelSequenceActor>(ALevelSequenceActor::StaticClass());

        if (LevelSequenceActor2)
        {
            LevelSequenceActor2->SetSequence(LevelSequence2);

            // Play the second sequence
            LevelSequenceActor2->GetSequencePlayer()->Play();
        }
    }
}

//------------------------------------------------------------
//-UI
#pragma region UI

void ARGameState::LoadLevelUI()
{
    FName LevelName = *UGameplayStatics::GetCurrentLevelName(this);

    if (LevelUIMap.Contains(LevelName))
    {
        TSubclassOf<UUserWidget> UIClass = LevelUIMap[LevelName];

        if (UIClass)
        {
            if (CurrentUI)
            {
                CurrentUI->RemoveFromParent();
                CurrentUI = nullptr;
            }

            CurrentUI = CreateWidget<UUserWidget>(GetWorld(), UIClass);
            if (CurrentUI)
            {
                CurrentUI->AddToViewport();
            }
        }
    }
}

void ARGameState::UpdateUI()
{
    if (CurrentUI)
    {
        UUserWidget* CurrentUserWidget = CurrentUI;
        UTextBlock* WavesText = Cast<UTextBlock>(CurrentUserWidget->GetWidgetFromName(TEXT("Waves")));
        if (WavesText)
        {
            WavesText->SetText(FText::Format(FText::FromString("Wave: {0}"), FText::AsNumber(mWave + 1)));
        }

        UTextBlock* CurrencyText = Cast<UTextBlock>(CurrentUserWidget->GetWidgetFromName(TEXT("Currency")));
        if (CurrencyText)
        {
            CurrencyText->SetText(FText::AsNumber(mCurrency));
        }

        UTextBlock* HealthText = Cast<UTextBlock>(CurrentUserWidget->GetWidgetFromName(TEXT("HealthText")));
        if (HealthText)
        {
            HealthText->SetText(FText::Format(FText::FromString("{0}/{1}"), FText::AsNumber(mHp), FText::AsNumber(mMaxHp)));
        }

        UProgressBar* Exp = Cast<UProgressBar>(CurrentUserWidget->GetWidgetFromName(TEXT("Exp")));
        if (Exp)
        {
            Exp->SetPercent((float)mExp / (float)mMaxExp);
        }

        UProgressBar* Health = Cast<UProgressBar>(CurrentUserWidget->GetWidgetFromName(TEXT("Health")));
        if (Health)
        {
            Health->SetPercent((float)mHp / (float)mMaxHp);
        }

        UTextBlock* Level = Cast<UTextBlock>(CurrentUserWidget->GetWidgetFromName(TEXT("LevelText")));
        if (Level)
        {
            Level->SetText(FText::Format(FText::FromString("Lv.{0}"), FText::AsNumber(mLevel)));
        }
    }
}

void ARGameState::SetMouseForMenu()
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
    if (PlayerController)
    {
        FInputModeUIOnly InputMode;
        PlayerController->SetInputMode(InputMode);

        PlayerController->bShowMouseCursor = true;
    }
}

void ARGameState::SetMouseForGame()
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
    if (PlayerController)
    {
        FInputModeGameOnly InputMode;
        PlayerController->SetInputMode(InputMode);

        PlayerController->bShowMouseCursor = false;
    }
}

void ARGameState::SetMouseForGame(UObject* WorldContextObject)
{
    ARGameState* GameState = GetGameState(WorldContextObject);
    if (GameState)
    {
        APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GameState, 0);
        if (PlayerController)
        {
            FInputModeGameOnly InputMode;
            PlayerController->SetInputMode(InputMode);

            PlayerController->bShowMouseCursor = false;
        }
    }
}

ARGameState* ARGameState::GetGameState(UObject* WorldContextObject)
{
    if (!WorldContextObject)
    {
        return nullptr;
    }
    return Cast<ARGameState>(UGameplayStatics::GetGameState(WorldContextObject));
}

void ARGameState::AddExp(UObject* WorldContextObject, int Amount)
{
    ARGameState* GameState = GetGameState(WorldContextObject);
    if (GameState)
    {
        GameState->mExp += Amount;

        if (GameState->mExp >= GameState->mMaxExp)
        {
            GameState->mExp = GameState->mExp - GameState->mMaxExp;
            GameState->mMaxExp += (30 * GameState->mLevel);
            GameState->mLevel++;
            GameState->mLevelsGottenDuringWave++;
            if (GameState->mExp >= GameState->mMaxExp) {
                GameState->AddExp(WorldContextObject, 0);
            }
        }

        GameState->UpdateUI();
    }
}

void ARGameState::AddCurrency(UObject* WorldContextObject, int Amount)
{
    ARGameState* GameState = GetGameState(WorldContextObject);
    if (GameState)
    {
        GameState->mCurrency += Amount;
        GameState->UpdateUI();
    }
}

void ARGameState::SetMaxHp(UObject* WorldContextObject, int Amount)
{
    ARGameState* GameState = GetGameState(WorldContextObject);
    if (GameState)
    {
        GameState->mMaxHp = Amount;
        GameState->UpdateUI();
    }
}

void ARGameState::SetHp(UObject* WorldContextObject, int Amount)
{
    ARGameState* GameState = GetGameState(WorldContextObject);
    if (GameState)
    {
        GameState->mHp = Amount;
        GameState->UpdateUI();
    }
}

void ARGameState::LevelUP()
{
    if (mLevelsGottenDuringWave > 0)
    {
        UUserWidget* levelSuperUI = CreateWidget<UUserWidget>(GetWorld(), LevelUPUI);
        if (levelSuperUI && mLevelsGottenDuringWave)
        {
            levelSuperUI->AddToViewport();
            SetMouseForMenu();
        }
    }
    else {
        OpenShop();
        isPaused = true;
    }
}

void ARGameState::OpenShop()
{
    UUserWidget* levelSuperUI = CreateWidget<UUserWidget>(GetWorld(), ShopUI);
    if (levelSuperUI)
    {
        levelSuperUI->AddToViewport();
        SetMouseForMenu();
        //UGameplayStatics::SetGamePaused(GetWorld(), true);
    }
}

void ARGameState::OpenMenu(UObject* WorldContextObject)
{
    ARGameState* GameState = GetGameState(WorldContextObject);
    if (GameState)
    {
        UUserWidget* settingsUI = CreateWidget<UUserWidget>(GameState->GetWorld(), GameState->SettingsUI);
        if (settingsUI)
        {
            settingsUI->AddToViewport();
            GameState->SetMouseForMenu();
            //UGameplayStatics::SetGamePaused(GetWorld(), true);
        }
        UGameplayStatics::SetGamePaused(GameState->GetWorld(), true);
    }
}

void ARGameState::Death(UObject* WorldContextObject)
{
    ARGameState* GameState = GetGameState(WorldContextObject);
    if (GameState)
    {
        UUserWidget* deathUI = CreateWidget<UUserWidget>(GameState->GetWorld(), GameState->DeathUI);
        if (deathUI)
        {
            deathUI->AddToViewport();
            GameState->SetMouseForMenu();
            //UGameplayStatics::SetGamePaused(GetWorld(), true);
        }
        UGameplayStatics::SetGamePaused(GameState->GetWorld(), true);
    }
}

void ARGameState::BuySword()
{
    AActor* player = UGameplayStatics::GetPlayerPawn(this, 0);
    ABratatouilleCharacter* character = Cast<ABratatouilleCharacter>(player);

    if (character)
    {
        if (mCurrency >= 150 && character->mWeapons.Num() < 6)
        {
            // Load the Blueprint class dynamically
            UClass* SwordBP = StaticLoadClass(AActor::StaticClass(), nullptr, TEXT("/Game/Characters/MyRSword.MyRSword_C"));

            if (SwordBP)
            {
                FActorSpawnParameters SpawnInfo;
                AActor* SpawnedSword = GetWorld()->SpawnActor<AActor>(SwordBP, FVector(0.f, 0.f, 0.f), FRotator(0.f, 0.f, 0.f), SpawnInfo);

                if (SpawnedSword)
                {
                    character->AddWeapon(Cast<ARSword>(SpawnedSword));
                    mCurrency -= 150;
                    UpdateUI();
                }
            }
        }
    }
}

void ARGameState::BuyGun()
{
    AActor* player = UGameplayStatics::GetPlayerPawn(this, 0);
    ABratatouilleCharacter* character = Cast<ABratatouilleCharacter>(player);
    if (character) {
        if (mCurrency >= 100 && character->mWeapons.Num() < 6)
        {
            FActorSpawnParameters SpawnInfo;
            ARGun* test1 = GetWorld()->SpawnActor<ARGun>(FVector(0.f, 0.f, 0.f), FRotator(0.f, 0.f, 0.f), SpawnInfo);
            character->AddWeapon(test1);
            mCurrency -= 100;
            UpdateUI();
        }
    }
}

#pragma endregion
//------------------------------------------------------------

//------------------------------------------------------------
//Spawner
#pragma region Spawn
void ARGameState::RemoveFromLifeEnemyList(UObject* WorldContextObject, AREnemy* enemy)
{
    ARGameState* GameState = GetGameState(WorldContextObject);
    if (enemy)
        GameState->mLifeEnemy.Remove(enemy);
}

void ARGameState::Spawn()
{
    int AmountOfPoint = SpawnPoints->SpawnPoints.Num();
    int random = FMath::RandRange(0, AmountOfPoint - 1);
    AREnemy* enemy = GetWorld()->SpawnActor<AREnemy>(EnemyWaves[mCurrWave].EnemyList[mCurrSpawnId], FTransform(SpawnPoints->SpawnPoints[random]));

    if (enemy)
    {
        mCurrSpawnId++;
        mLifeEnemy.Add(enemy);
        enemy->Init(EnemyWaves[mCurrWave].Level);
    }

    mCanSpawn = false;
    GetWorldTimerManager().ClearAllTimersForObject(this);
    FTimerHandle timerHandle;
    GetWorldTimerManager().SetTimer(timerHandle, this, &ARGameState::SpawnDelay, mSpawnDelay, false);
}

void ARGameState::SpawnDelay()
{
    mCanSpawn = true;
    GetWorldTimerManager().ClearAllTimersForObject(this);
}
#pragma endregion
//------------------------------------------------------------