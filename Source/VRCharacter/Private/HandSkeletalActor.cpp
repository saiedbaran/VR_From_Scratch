// Fill out your copyright notice in the Description page of Project Settings.


#include "HandSkeletalActor.h"

#include "VRHandAnimationInstance.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Interfaces/Interactable.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AHandSkeletalActor::AHandSkeletalActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Base = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Base->SetupAttachment(RootComponent);

	HandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HandMesh"));
	HandMesh->SetupAttachment(Base);

	GrabSphere = CreateDefaultSubobject<USphereComponent>(TEXT("GrabSphere"));
	GrabSphere->SetupAttachment(HandMesh);
	GrabSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GrabSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	GrabSphere->OnComponentBeginOverlap.AddDynamic(this, &AHandSkeletalActor::OnGrabSphereBeginOverlap);
	GrabSphere->OnComponentEndOverlap.AddDynamic(this, &AHandSkeletalActor::OnGrabSphereEndOverlap);

	DebugComponents = CreateDefaultSubobject<USceneComponent>(TEXT("DebugComponents"));
	DebugComponents->SetupAttachment(HandMesh);
}

void AHandSkeletalActor::RotateDebugWidgetToCamera()
{
	if (!DebugWidgetComponent) { return; }

	auto lookDirection = UKismetMathLibrary::FindLookAtRotation(
		DebugWidgetComponent->GetComponentLocation(),
		UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation());

	DebugWidgetComponent->SetWorldRotation(lookDirection);
}

// Called when the game starts or when spawned
void AHandSkeletalActor::BeginPlay()
{
	Super::BeginPlay();

	HandAnimationInstance = Cast<UVRHandAnimationInstance>(HandMesh->GetAnimInstance());
	if (HandAnimationInstance)
	{
		HandAnimationInstance->IsInDefaultPose = 0;
	}
}

// Called every frame
void AHandSkeletalActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bDebugVisualizationEnabled) { RotateDebugWidgetToCamera(); }
}

void AHandSkeletalActor::ResetPoseToDefault()

{
	if (!HandAnimationInstance) { return; }
	HandAnimationInstance->IsInDefaultPose = 0;
	HandAnimationInstance->IsInDefaultPose = 1;
}

void AHandSkeletalActor::UpdateTeleportPose(float Alpha)
{
	if (!HandAnimationInstance) { return; }
	HandAnimationInstance->PoseAlphaTeleport = Alpha;
}

void AHandSkeletalActor::GrabPressed(UMotionControllerComponent* MotionController)
{
	if (const auto interactable = Cast<IInteractable>(ReadyToGrabActor))
	{
		interactable->GrabPressed(this);
		AttachedActor = ReadyToGrabActor;
	}
}

void AHandSkeletalActor::GrabReleased(UMotionControllerComponent* MotionController)
{
	if (const auto interactable = Cast<IInteractable>(AttachedActor))
	{
		interactable->GrabReleased(this);
		AttachedActor = nullptr;
		ReadyToGrabActor = nullptr;
		ReadyToGrabComponent = nullptr;
	}
}

void AHandSkeletalActor::UpdateGrabPose(float Alpha)
{
	if (!HandAnimationInstance) { return; }

	HandAnimationInstance->IsInDefaultPose = 0;
	HandAnimationInstance->PoseAlphaGrab = Alpha;
}


void AHandSkeletalActor::OnGrabSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                                  const FHitResult& SweepResult)
{
	if (OtherActor->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
	{
		UpdateDebugText("Overlapped Actor", OtherActor->GetName());
		ReadyToGrabActor = OtherActor;
		ReadyToGrabComponent = OtherComp;
	}
}

void AHandSkeletalActor::OnGrabSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

	if(ReadyToGrabActor != nullptr && OtherActor == ReadyToGrabActor)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
						 FString::Printf(TEXT("End Overlap: %s"), *OtherActor->GetName()));
		UpdateDebugText("Overlapped Actor", "None");
		ReadyToGrabActor = nullptr;
		ReadyToGrabComponent = nullptr;
	}
}
