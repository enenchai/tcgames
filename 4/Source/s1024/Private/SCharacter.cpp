// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true; //可以上下移动鼠标控制镜头方向

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true; //可以蹲下




	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	//CameraComp->bUsePawnControlRotation = true;
	CameraComp->SetupAttachment(SpringArmComp);
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASCharacter::MoveForward(float Value)
{
	//参数传递：引擎 输入 轴映射 新建moveforward scale的值就是value的值
	//1 前进方向 2 向前1.0f 向后-1.0f 速度0.5f 3 默认false 可以不写
	AddMovementInput(GetActorForwardVector(), Value, false);
}

void ASCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector(), Value, false);
}

void ASCharacter::BeginCrouch()
{
	Crouch();
}

void ASCharacter::EndCrouch()
{
	UnCrouch();
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ASCharacter::AddScore()
{
	score++;
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//点一下动一下
	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);
	//pitch y轴 roll x轴 yaw z轴
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput); //左右移动镜头 绕z轴前向旋转
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput); //上下看

	//点一下保持状态
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ASCharacter::BeginCrouch); //蹲下
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ASCharacter::EndCrouch); //站起

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASCharacter::Jump);

}

FVector ASCharacter::GetPawnViewLocation() const //人物眼睛位置
{
	if (CameraComp)
	{
		return CameraComp->GetComponentLocation();
	}
	return Super::GetPawnViewLocation();
}

