#include "SGameMode.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Components/SHealthComponent.h"
#include "SGameState.h"

ASGameMode::ASGameMode()
{
    GameStateClass = ASGameMode::StaticClass();

    TimeBetweenWaves = 2.0f;
    WaveCount = 0;

    PrimaryActorTick.bCanEverTick = true; //tick被调用
    PrimaryActorTick.TickInterval = 1.0f; //tick每帧调用一次改为每秒调用一次
}


void ASGameMode::StartPlay()
{
    Super::StartPlay();
    PrepareForNextWave();

}

void ASGameMode::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    CheckWaveState();
    CheckAnyPlayerAlive();
}

void ASGameMode::StartWave()
{
    WaveCount++;
    NrOfBotsToSpawn = 2 * WaveCount;
    GetWorldTimerManager().SetTimer(TimerHandle_SpawnInterval, this, &ASGameMode::SpawnBotTimerElapsed, 1.0f, true, 0.0f);
    SetWaveState(EWaveState::WaveInProgress);
}

void ASGameMode::EndWave()
{
    GetWorldTimerManager().ClearTimer(TimerHandle_SpawnInterval);
    SetWaveState(EWaveState::WaitingToComplete);
}

void ASGameMode::PrepareForNextWave()
{
    GetWorldTimerManager().SetTimer(TimerHandle_NextWaveStart, this, &ASGameMode::StartWave, TimeBetweenWaves, false);
    SetWaveState(EWaveState::WaitingToStart);
    RestartDeadPlayers();
}

void ASGameMode::SpawnBotTimerElapsed()
{
    SpawnNewBot();
    NrOfBotsToSpawn--;
    if (NrOfBotsToSpawn <= 0) {
        EndWave();
    }
}

void ASGameMode::CheckWaveState()
{
    bool bIsPreparingForWave = GetWorldTimerManager().IsTimerActive(TimerHandle_NextWaveStart);;

    if (NrOfBotsToSpawn > 0 || bIsPreparingForWave) {
        return;
    }

    bool bIsAnyBotAlive = false;

    for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
    {
        APawn* TestPawn = It->Get();
        if (TestPawn->GetFName().ToString().Contains("Bot"))
        {
            USHealthComponent* HealthComp = Cast<USHealthComponent>(TestPawn->GetComponentByClass(USHealthComponent::StaticClass()));
            if (HealthComp&&HealthComp->GetCurrentHealth() > 0.0f)
            {
                bIsAnyBotAlive = true;
                break;
            }
        }
    }

    if (!bIsAnyBotAlive) {
        SetWaveState(EWaveState::WaveComplete);
        PrepareForNextWave();
    }
}

void ASGameMode::CheckAnyPlayerAlive()
{
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It) {
        APlayerController* PC = It->Get();
        if (PC&&PC->GetPawn()) {
            APawn* MyPawn = PC->GetPawn();
            USHealthComponent* HealthComp = Cast<USHealthComponent>(MyPawn->GetComponentByClass(USHealthComponent::StaticClass()));
            if (HealthComp&&HealthComp->GetCurrentHealth() > 0.0f) {
                return;
            }
        }
    }

    RestartDeadPlayers();
    GameOver();
}

void ASGameMode::GameOver()
{
    EndWave();
    SetWaveState(EWaveState::GameOver);
    UE_LOG(LogTemp, Warning, TEXT("Player has died"));
}

void ASGameMode::SetWaveState(EWaveState NewState)
{
    ASGameState* GS = GetGameState<ASGameState>();

    if (GS)
    {
        GS->SetWaveState(NewState);
    }
}

void ASGameMode::RestartDeadPlayers()
{
    for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It) {
        APlayerController* PC = It->Get();
        if (PC&&PC->GetPawn() == nullptr)
        {
            RestartPlayer(PC);
        }
    }
}
