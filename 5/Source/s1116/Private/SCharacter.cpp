// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "SWeapon.h"
#include "Engine/World.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    
    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
    SpringArmComp->SetupAttachment(RootComponent);//设为子物体
    SpringArmComp->bUsePawnControlRotation = true;
    
    GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
    
    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
    //CameraComp->bUsePawnControlRotation = true;
    CameraComp->SetupAttachment(SpringArmComp);//设为子物体
    
    ZoomedFov = 65.0f;
    ZoomInterSpeed = 20.0f;
}

void ASCharacter::MoveForward(float Value)
{
    AddMovementInput(GetActorForwardVector(),Value);
}

void ASCharacter::MoveRight(float Value)
{
    AddMovementInput(GetActorRightVector(),Value);
}

void ASCharacter::BeginCrouch()
{
    Crouch();
}

void ASCharacter::EndCrouch()
{
    UnCrouch();
}

void ASCharacter::StartZoom()
{
    bWantsToZoom = true;
}

void ASCharacter::EndZoom()
{
    bWantsToZoom = false;
}

void ASCharacter::StartFire()
{
    CurrentWeapon->StartFire();
}

void ASCharacter::EndFire()
{
    CurrentWeapon->EndFire();
}


// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
    DefaultFov = CameraComp->FieldOfView;
    
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    CurrentWeapon = GetWorld()->SpawnActor<ASWeapon>(StarterWeaponClass,FVector::ZeroVector,FRotator::ZeroRotator,SpawnParams);
    
    if(CurrentWeapon)
    {
        CurrentWeapon->SetOwner(this);
        CurrentWeapon->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale,"WeaponSocket");
    }
}


// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
    float CurrentFov = bWantsToZoom?ZoomedFov:DefaultFov;
    
    float DiveIntoCurrentFov = FMath::FInterpTo(CameraComp->FieldOfView,CurrentFov,DeltaTime,ZoomInterSpeed);
    
    CameraComp->SetFieldOfView(DiveIntoCurrentFov);
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    
    //持续操作 轴映射
    PlayerInputComponent->BindAxis("MoveForward",this,&ASCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight",this,&ASCharacter::MoveRight);
    
    //绕z轴旋转 左右看
    PlayerInputComponent->BindAxis("Turn",this,&ASCharacter::AddControllerYawInput);
    //绕y轴旋转 上下看
    PlayerInputComponent->BindAxis("LookUp",this,&ASCharacter::AddControllerPitchInput);
    
    PlayerInputComponent->BindAction("Crouch",IE_Pressed,this,&ASCharacter::BeginCrouch);
    //IE_PRESSED按下
    PlayerInputComponent->BindAction("Crouch",IE_Released,this,&ASCharacter::EndCrouch);
    
    PlayerInputComponent->BindAction("Jump",IE_Pressed,this,&ASCharacter::Jump);
    
    PlayerInputComponent->BindAction("Zoom",IE_Pressed,this,&ASCharacter::StartZoom);
    PlayerInputComponent->BindAction("Zoom",IE_Released,this,&ASCharacter::EndZoom);
    
    PlayerInputComponent->BindAction("Fire",IE_Pressed,this,&ASCharacter::StartFire);
    PlayerInputComponent->BindAction("Fire",IE_Released,this,&ASCharacter::EndFire);
}

FVector ASCharacter::GetPawnViewLocation() const
{
    if(CameraComp)
    {
        return CameraComp->GetComponentLocation();
    }
    return Super::GetPawnViewLocation();
}

