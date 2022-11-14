// Fill out your copyright notice in the Description page of Project Settings.


#include "VRCharacterBase.h"
#include "CineCameraComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AVRCharacterBase::AVRCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VROffset = CreateDefaultSubobject<USceneComponent>("VROffset");
	VROffset->SetupAttachment(GetCapsuleComponent());
	VROffset->SetRelativeLocation(FVector(0, 0, -1 * GetCapsuleComponent()->GetScaledCapsuleHalfHeight()));

	VRCamera = CreateDefaultSubobject<UCineCameraComponent>("VRCamera");
	VRCamera->SetupAttachment(VROffset);

	ControllerBase = CreateDefaultSubobject<USceneComponent>(TEXT("ControllerBase"));
	ControllerBase->SetupAttachment(VROffset);

	RightMotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightMotionController"));
	RightMotionController->SetupAttachment(ControllerBase);
	RightMotionController->SetTrackingMotionSource(FName("Right"));

	LeftMotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftMotionController"));
	LeftMotionController->SetupAttachment(ControllerBase);
	LeftMotionController->SetTrackingMotionSource(FName("Left"));
	
}

// Called when the game starts or when spawned
void AVRCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void AVRCharacterBase::CharacterMoveForward(float ratio)
{
	const auto cameraProjectedForward =
		FVector(VRCamera->GetForwardVector().X, VRCamera->GetForwardVector().Y, 0).GetSafeNormal();
	AddActorWorldOffset(cameraProjectedForward * ratio * MaximumSpeed);
}

void AVRCharacterBase::FindFocusDistance()
{
	FVector start = VRCamera->GetComponentLocation();
	FVector end = start + VRCamera->GetForwardVector()*MaximumFocusTrackingDistance;
	FHitResult hitResult;
	FCameraFocusSettings focusSettings;
	focusSettings.ManualFocusDistance = MaximumFocusTrackingDistance;
	if(!GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECC_Visibility)) {return;}
	
	focusSettings.ManualFocusDistance = hitResult.Distance;
	VRCamera->SetFocusSettings(focusSettings);

}

// Called every frame
void AVRCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FindFocusDistance();
}

// Called to bind functionality to input
void AVRCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
