#include "Components/Teleportation.h"

#include "CineCameraComponent.h"
#include "NiagaraComponent.h"
#include "TeleportationTrace.h"
#include "Components/TeleportPreventedArea.h"
#include "VRCharacterBase.h"
#include "Components/SplineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values for this component's properties
UTeleportation::UTeleportation()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTeleportation::BeginPlay()
{
	Super::BeginPlay();

	VRCharacter = Cast<AVRCharacterBase>(GetOwner());
}


// Called every frame
void UTeleportation::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsTracingForTeleportLocation)
	{
		TraceForTeleportLocation();
	}
}

void UTeleportation::BeginTeleport(float ratio)
{
	// start tracing for the teleport location
	bIsTracingForTeleportLocation = true;

	VRCharacter->UpdateRightHandPose(ratio);
}

void UTeleportation::EndTeleport()
{
	bIsTracingForTeleportLocation = false;

	VRCharacter->TeleportLocationIndicator->SetHiddenInGame(true);
	VRCharacter->TeleportPreventionIndicator->SetHiddenInGame(true);
	VRCharacter->UpdateRightHandPose(0.0f);

	if (TeleportTrace) { TeleportTrace->NiagaraComponent->SetHiddenInGame(true); }
	if (bFoundTeleportLocation) {TeleportAction();}
}

void UTeleportation::TraceForTeleportLocation()
{
	// active tracing visualization (Niagara system)

	FPredictProjectilePathParams predictParams;
	predictParams.bTraceComplex = false;
	predictParams.bTraceWithCollision = true;
	predictParams.bTraceWithChannel = true;
	predictParams.TraceChannel = ECC_Visibility;

	// TODO: make it work for both hands
	const auto startLocation = VRCharacter->RightMotionController->GetComponentLocation();
	predictParams.StartLocation = startLocation;
	predictParams.LaunchVelocity = TeleportTraceProjectileVelocity * VRCharacter->RightMotionController->
		GetForwardVector();

	// Projectile Prediction results
	FPredictProjectilePathResult predictResult;

	if (UGameplayStatics::PredictProjectilePath(this, predictParams, predictResult))
	{
		// Debugging location
		/*if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green,
			                                 FString::Printf(
				                                 TEXT("Number of Predicted Points: %d"),
				                                 predictResult.PathData.Num()));
		}*/

		TeleportLocation = predictResult.HitResult.Location;
		auto camera2DForward = UKismetMathLibrary::ProjectVectorOnToPlane(
			VRCharacter->VRCamera->GetForwardVector(), FVector(0, 0, 1));

		if (TeleportTrace)
		{
			TeleportTrace->NiagaraComponent->SetHiddenInGame(false);
			TeleportTrace->UpdateSplineLocationTangent(predictResult.PathData[0].Location,
			                                           TeleportLocation,
			                                           VRCharacter->RightMotionController->GetForwardVector(),
			                                           FVector(0, 0, 1));
		}

		else
		{
			// Debug Only
			UE_LOG(LogTemp, Error, TEXT("TeleportTrace is null"));
		}

		// Check if it is possible to teleport to the found location
		if (predictResult.HitResult.GetActor()->GetComponentByClass(UTeleportPreventedArea::StaticClass()))
		{
			VRCharacter->TeleportLocationIndicator->SetHiddenInGame(true);
			VRCharacter->TeleportPreventionIndicator->SetHiddenInGame(false);

			VRCharacter->TeleportPreventionIndicator->SetWorldLocation(TeleportLocation);
			VRCharacter->TeleportPreventionIndicator->SetWorldRotation(
				UKismetMathLibrary::MakeRotFromX(camera2DForward));

			bFoundTeleportLocation = false;

			return;
		}


		bFoundTeleportLocation = true;


		VRCharacter->TeleportPreventionIndicator->SetHiddenInGame(true);
		VRCharacter->TeleportLocationIndicator->SetHiddenInGame(false);

		VRCharacter->TeleportLocationIndicator->SetWorldLocation(TeleportLocation);
		VRCharacter->TeleportLocationIndicator->SetWorldRotation(
			UKismetMathLibrary::MakeRotFromX(camera2DForward));
	}

	else
	{
		// Debug

		/*GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red,
		                                 FString::Printf(
			                                 TEXT("No Hit")));*/

		VRCharacter->TeleportLocationIndicator->SetHiddenInGame(true);
		VRCharacter->TeleportPreventionIndicator->SetHiddenInGame(true);

		if (TeleportTrace) { TeleportTrace->NiagaraComponent->SetHiddenInGame(true); }
		bFoundTeleportLocation = false;
	}
}

void UTeleportation::TeleportAction()
{
	UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->
		StartCameraFade(0, 1, TeleportDelay, FLinearColor::Black);

	FTimerHandle FadeTimer;
	GetWorld()->GetTimerManager().SetTimer(FadeTimer, this, &UTeleportation::TeleportCharacter, TeleportDelay);
}

void UTeleportation::TeleportCharacter()
{
	VRCharacter->SetActorLocation(TeleportLocation);
	UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->
		StartCameraFade(1, 0, TeleportDelay, FLinearColor::Black);
}
