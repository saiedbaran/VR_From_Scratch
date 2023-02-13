// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MotionControllerComponent.h"
#include "VRCharacterStatics.h"
#include "GameFramework/Actor.h"
#include "HandSkeletalActor.generated.h"

class USphereComponent;
class UWidgetComponent;
class UVRHandAnimationInstance;
UCLASS()
class VRCHARACTER_API AHandSkeletalActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHandSkeletalActor();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void ResetPoseToDefault();

	UFUNCTION(BlueprintCallable)
	void UpdateTeleportPose(float Alpha);


	/**
	 * @brief Grab Interaction
	 */
	UFUNCTION()
	void OnGrabSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                              const FHitResult& SweepResult);
	UFUNCTION()
	void OnGrabSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION(BlueprintCallable, Category = "Interactions|Grab")
	void GrabPressed(UMotionControllerComponent* MotionController);
	UFUNCTION(BlueprintCallable, Category = "Interactions|Grab")
	void GrabReleased(UMotionControllerComponent* MotionController);

	UFUNCTION(BlueprintCallable)
	void UpdateGrabPose(float Alpha);

	/**
	 * @brief 
	 * @param VariableName name of the variable to be displayed
	 * @param VariableValue value of the variable to be displayed
	 */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Debug")
	void UpdateDebugText(const FString& VariableName, const FString& VariableValue);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Debug")
	void ToggleDebugVisualization();

	UFUNCTION(BlueprintCallable, Category = "Debug")
	void RotateDebugWidgetToCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* Base;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USkeletalMeshComponent* HandMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UVRHandAnimationInstance* HandAnimationInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interactions|Properties")
	TEnumAsByte<EHandType> HandType = EHandType::RightHand;

	/**
	 * @brief Grab Interaction
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interactions|Grab")
	USphereComponent* GrabSphere;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interactions|Grab")
	bool bUseGrabSphere = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interactions|Grab")
	AActor* ReadyToGrabActor = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interactions|Grab")
	USceneComponent* ReadyToGrabComponent = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interactions|Grab")
	AActor* AttachedActor = nullptr;
	

	/**
	 * @brief Debug Components and members
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	USceneComponent* DebugComponents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	UWidgetComponent* DebugWidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Debug")
	bool bDebugVisualizationEnabled = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Debug")
	float LastDebugToggleTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
	float DelayBetweenToggleDebug = 5.0f;
};
