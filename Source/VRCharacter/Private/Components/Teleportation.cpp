#include "Components/Teleportation.h"

#include "CineCameraComponent.h"
#include "VRCharacterBase.h"
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
	VRCharacter->UpdateRightHandPose(0.0f);
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
				                                 TEXT("Predicted Location: %s"),
				                                 *predictResult.HitResult.Location.ToString()));
		}*/

		VRCharacter->TeleportLocationIndicator->SetWorldLocation(predictResult.HitResult.Location);
		VRCharacter->TeleportLocationIndicator->SetHiddenInGame(false);
		auto camera2DForward = UKismetMathLibrary::ProjectVectorOnToPlane(
			VRCharacter->VRCamera->GetForwardVector(), FVector(0, 0, 1));
		VRCharacter->TeleportLocationIndicator->SetWorldRotation(
			UKismetMathLibrary::MakeRotFromX(camera2DForward));
	}

	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red,
		                                 FString::Printf(
			                                 TEXT("No Hit")));
		VRCharacter->TeleportLocationIndicator->SetHiddenInGame(true);
	}
}
