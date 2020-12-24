// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SGameMode.generated.h"

enum class EWaveState :uint8;

UCLASS()
class S1116_API ASGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:

    ASGameMode();

    int32 NrOfBotsToSpawn;//本波生成多少敌人
    int32 WaveCount;//第几波
    float TimeBetweenWaves;//波与波之间的时间间隔

    FTimerHandle TimerHandle_SpawnInterval;
    FTimerHandle TimerHandle_NextWaveStart;

    //在蓝图里，生成一个低级的敌人
        UFUNCTION(BlueprintImplementableEvent,Category="GameMode")
        void SpawnNewBot();

        //开始生成一波敌人
        void StartWave();

        //一波结束
        void EndWave();

        //为下一波设定时间，确定多久生成下一波
        void PrepareForNextWave();

        void SpawnBotTimerElapsed();

        void CheckWaveState();

        void CheckAnyPlayerAlive();

        void GameOver();

        void SetWaveState(EWaveState NewState);

        void RestartDeadPlayers();
public:
    virtual void StartPlay() override;
    virtual void Tick(float DeltaSeconds) override;
    
};
