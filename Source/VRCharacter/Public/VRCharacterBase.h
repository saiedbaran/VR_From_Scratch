// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MotionControllerComponent.h"
#include "GameFramework/Character.h"
#include "VRCharacterBase.generated.h"

class AHandSkeletalActor;
class UCineCameraComponent;

UCLASS()
class VRCHARACTER_API AVRCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVRCharacterBase();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void UpdateRightHandPose(float teleportRatio = 0, float grabRatio = 0, bool updateTeleportPose = true,
										   bool updateGrabPose = false);
	UFUNCTION(BlueprintCallable)
	void UpdateLeftHandPose(float teleportRatio = 0, float grabRatio = 0, bool updateTeleportPose = true,
										   bool updateGrabPose = true);

	/**
	 * @brief Interaction functions
	 */
	UFUNCTION(BlueprintCallable)
	void RightGrabPressed();
	UFUNCTION(BlueprintCallable)
	void LeftGrabPressed();
	UFUNCTION(BlueprintCallable)
	void RightGrabReleased();
	UFUNCTION(BlueprintCallable)
	void LeftGrabReleased();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void CharacterMoveForward(float ratio);

	UFUNCTION(BlueprintCallable)
	void FindFocusDistance();

	void SpawnHandSkeletalMeshActors();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USceneComponent* VROffset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UCineCameraComponent* VRCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USceneComponent* ControllerBase;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UMotionControllerComponent* RightMotionController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	TSubclassOf<AHandSkeletalActor> RightHandActorTemplate;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UMotionControllerComponent* LeftMotionController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	TSubclassOf<AHandSkeletalActor> LeftHandActorTemplate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components|Teleportation")
	UStaticMeshComponent* TeleportLocationIndicator;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components|Teleportation")
	UStaticMeshComponent* TeleportPreventionIndicator;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Locomotion|Movement")
	float MaximumSpeed = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Locomotion|Movement")
	float MaximumFocusTrackingDistance = 10000.0f;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components|Hands")
	AHandSkeletalActor* RightHand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	USkeletalMeshComponent* RightHandPlaceHolder;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components|Hands")
	AHandSkeletalActor* LeftHand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	USkeletalMeshComponent* LeftHandPlaceHolder;
};
