// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class ASWeapon;

UCLASS()
class S1116_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    void MoveForward(float Value);
    void MoveRight(float Value);
    void BeginCrouch();
    void EndCrouch();
    void StartZoom();
    void EndZoom();
    void StartFire();
    void EndFire();
    
    UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
    UCameraComponent* CameraComp;
    
    UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Components")
    USpringArmComponent* SpringArmComp;
    
    bool bWantsToZoom;
    float DefaultFov;//摄像机角度
    UPROPERTY(EditDefaultsOnly,Category="Player")
    float ZoomedFov;//开镜摄像机角度
    
    UPROPERTY(EditDefaultsOnly,Category="Player",meta=(ClampMin=0.1,ClampMax=100))
    float ZoomInterSpeed;//开镜摄像机角度变化速度
    
    ASWeapon* CurrentWeapon;
    
    UPROPERTY(EditDefaultsOnly,Category="Player")
    TSubclassOf<ASWeapon> StarterWeaponClass;
    
    
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    
    virtual FVector GetPawnViewLocation() const override;
    
};
