// Fill out your copyright notice in the Description page of Project Settings.


#include "VRCharacterBase.h"
#include "CineCameraComponent.h"
#include "HandSkeletalActor.h"
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

	// Create Motion Controllers
	RightMotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightMotionController"));
	RightMotionController->SetupAttachment(ControllerBase);
	RightMotionController->SetTrackingMotionSource(FName("Right"));

	LeftMotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftMotionController"));
	LeftMotionController->SetupAttachment(ControllerBase);
	LeftMotionController->SetTrackingMotionSource(FName("Left"));

	// Create placeholder for hands
	RightHandPlaceHolder = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RightHandPlaceHolder"));
	RightHandPlaceHolder->SetupAttachment(RightMotionController);
	RightHandPlaceHolder->SetHiddenInGame(true);

	LeftHandPlaceHolder = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LeftHandPlaceHolder"));
	LeftHandPlaceHolder->SetupAttachment(LeftMotionController);
	LeftHandPlaceHolder->SetHiddenInGame(true);

	// create teleportation components
	TeleportLocationIndicator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TeleportLocationIndicator"));
	TeleportLocationIndicator->SetupAttachment(RootComponent);
	TeleportLocationIndicator->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TeleportLocationIndicator->SetHiddenInGame(true);

	TeleportPreventionIndicator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TeleportPreventionIndicator"));
	TeleportPreventionIndicator->SetupAttachment(RootComponent);
	TeleportPreventionIndicator->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TeleportPreventionIndicator->SetHiddenInGame(true);
}

// Called when the game starts or when spawned
void AVRCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	SpawnHandSkeletalMeshActors();
}

void AVRCharacterBase::SpawnHandSkeletalMeshActors()
{
	RightHand = Cast<AHandSkeletalActor>(
		GetWorld()->SpawnActor<AActor>(RightHandActorTemplate, RightHandPlaceHolder->GetComponentTransform()));
	if (RightHand)
	{
		RightHand->AttachToComponent(RightMotionController, FAttachmentTransformRules::KeepWorldTransform);
		RightHand->SetOwner(this);
		RightHand->HandType = EHandType::RightHand;
	}

	LeftHand = Cast<AHandSkeletalActor>(
		GetWorld()->SpawnActor<AActor>(LeftHandActorTemplate, LeftHandPlaceHolder->GetComponentTransform()));
	if (LeftHand)
	{
		LeftHand->AttachToComponent(LeftMotionController, FAttachmentTransformRules::KeepWorldTransform);
		LeftHand->SetOwner(this);
		LeftHand->HandType = EHandType::LeftHand;
	}
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
	FVector end = start + VRCamera->GetForwardVector() * MaximumFocusTrackingDistance;
	FHitResult hitResult;
	FCameraFocusSettings focusSettings;
	focusSettings.ManualFocusDistance = MaximumFocusTrackingDistance;
	if (!GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECC_Visibility)) { return; }

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

void AVRCharacterBase::UpdateRightHandPose(float teleportRatio, float grabRatio, bool updateTeleportPose,
                                           bool updateGrabPose)
{
	if(teleportRatio < 0.01f && grabRatio < 0.01f)
	{
		RightHand->ResetPoseToDefault();
		return;
	}
	
	if(updateTeleportPose)
	{
		RightHand->UpdateTeleportPose(teleportRatio);
	}
	if(updateGrabPose)
	{
		RightHand->UpdateGrabPose(grabRatio);
	}
}

void AVRCharacterBase::UpdateLeftHandPose(float teleportRatio, float grabRatio, bool updateTeleportPose,
										   bool updateGrabPose)
{
}

void AVRCharacterBase::RightGrabPressed()
{
	RightHand->GrabPressed(RightMotionController);
}

void AVRCharacterBase::LeftGrabPressed()
{
}

void AVRCharacterBase::RightGrabReleased()
{
	RightHand->GrabReleased(RightMotionController);
}

void AVRCharacterBase::LeftGrabReleased()
{
}
