// Fill out your copyright notice in the Description page of Project Settings.


#include "HandSkeletalActor.h"

#include "NiagaraComponent.h"
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

	GrabbityHandEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("GrabbityHandEffect"));
	GrabbityHandEffect->SetupAttachment(HandMesh);
	GrabbityHandEffect->SetHiddenInGame(true);

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
	if (bIsGrabbityInteractionEnabled) { TraceForGrabbityActor(); }
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
	if (InteractionMode == EInteractionMode::Proximity)
	{
		if (const auto interactable = Cast<IInteractable>(ReadyToProximityGrabActor))
		{
			interactable->GrabPressed(this);
			AttachedProximityActor = ReadyToProximityGrabActor;
		}
	}
	else if (InteractionMode == EInteractionMode::Grabbity)
	{
		if (const auto interactable = Cast<IInteractable>(ReadyToGrabbityGrabActor))
		{
			interactable->GrabbityGrabPressed(this);
			AttachedGrabbityActor = ReadyToGrabbityGrabActor;
			GrabbityHandEffect->SetHiddenInGame(true);
		}
	}
}

void AHandSkeletalActor::GrabReleased(UMotionControllerComponent* MotionController)
{
	if (InteractionMode == EInteractionMode::Proximity)
	{
		if (const auto interactable = Cast<IInteractable>(AttachedProximityActor))
		{
			interactable->GrabReleased(this);
			AttachedProximityActor = nullptr;
			ReadyToProximityGrabActor = nullptr;
			ReadyToProximityGrabComponent = nullptr;
		}
	}
	else if (InteractionMode == EInteractionMode::Grabbity)
	{
		if (const auto interactable = Cast<IInteractable>(ReadyToGrabbityGrabActor))
		{
			interactable->GrabbityGrabReleased(this);
			AttachedGrabbityActor = nullptr;
			ReadyToGrabbityGrabActor = nullptr;
			ReadyToGrabbityGrabComponent = nullptr;

			GrabbityHandEffect->SetHiddenInGame(false);
		}
	}
}

void AHandSkeletalActor::TraceForGrabbityActor()
{
	auto camera = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	auto start = camera->GetCameraLocation();
	auto end = start + camera->GetActorForwardVector() * TraceMaxDistance;

	TArray<FHitResult> hitResults;
	auto debugTraceType = bShowDebugTrace ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None;
	UKismetSystemLibrary::SphereTraceMulti(GetWorld(), start, end, TraceRadius,
	                                       TraceChannel,
	                                       false, TArray<AActor*>(), debugTraceType, hitResults, true);

	if(hitResults.Num() == 0)
	{
		if (ReadyToGrabbityGrabActor) { Cast<IInteractable>(ReadyToGrabbityGrabActor)->GrabbityHoverEnd(this); }

        ReadyToGrabbityGrabActor = nullptr;
        ReadyToGrabbityGrabComponent = nullptr;
		return;
	}

	for (auto hit : hitResults)
	{
		if (hit.GetActor()->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
		{
			auto interactableActor = Cast<IInteractable>(hit.GetActor());
			if (interactableActor->IsGrabbityInteractable())
			{
				if (ReadyToGrabbityGrabActor) { Cast<IInteractable>(ReadyToGrabbityGrabActor)->GrabbityHoverEnd(this); }

				ReadyToGrabbityGrabActor = hit.GetActor();
				ReadyToGrabbityGrabComponent = hit.GetComponent();

				interactableActor->GrabbityHoverBegin(this);
				// GrabbityHandEffect->SetVectorParameter("user.Direction",
				//                                        (ReadyToGrabbityGrabActor->GetRootComponent()->GetComponentLocation() - HandMesh->
				// 	                                       GetComponentLocation()).GetSafeNormal());
				return;
			}
		}
		else
		{
			// GrabbityHandEffect->SetVectorParameter("user.Direction", HandMesh->GetUpVector() * -1);
			if (ReadyToGrabbityGrabActor) { Cast<IInteractable>(ReadyToGrabbityGrabActor)->GrabbityHoverEnd(this); }

			ReadyToGrabbityGrabActor = nullptr;
			ReadyToGrabbityGrabComponent = nullptr;
		}
	}
}

void AHandSkeletalActor::ToggleGrabbityGrabbing()
{
	if (bIsGrabbityInteractionEnabled)
	{
		InteractionMode = EInteractionMode::Proximity;
		if (EmissionPowers.Num() > 0)
		{
			DynamicMaterialInstance->SetScalarParameterValue("_emissionPower", EmissionPowers[0]);
		}
		if (OverlayColors.Num() > 0)
		{
			DynamicMaterialInstance->SetVectorParameterValue("_overlayColor", OverlayColors[0]);
		}

		GrabbityHandEffect->SetHiddenInGame(true);
		GrabbityHandEffect->SetVectorParameter("user.Direction", HandMesh->GetUpVector() * -1);
	}
	else
	{
		InteractionMode = EInteractionMode::Grabbity;
		if (EmissionPowers.Num() > 1)
		{
			DynamicMaterialInstance->SetScalarParameterValue("_emissionPower", EmissionPowers[1]);
		}
		if (OverlayColors.Num() > 1)
		{
			DynamicMaterialInstance->SetVectorParameterValue("_overlayColor", OverlayColors[1]);
		}

		GrabbityHandEffect->SetHiddenInGame(false);
	}

	bIsGrabbityInteractionEnabled = !bIsGrabbityInteractionEnabled;
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
		ReadyToProximityGrabActor = OtherActor;
		ReadyToProximityGrabComponent = OtherComp;
	}
}

void AHandSkeletalActor::OnGrabSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ReadyToProximityGrabActor != nullptr && OtherActor == ReadyToProximityGrabActor)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
		                                 FString::Printf(TEXT("End Overlap: %s"), *OtherActor->GetName()));
		UpdateDebugText("Overlapped Actor", "None");
		ReadyToProximityGrabActor = nullptr;
		ReadyToProximityGrabComponent = nullptr;
	}
}
