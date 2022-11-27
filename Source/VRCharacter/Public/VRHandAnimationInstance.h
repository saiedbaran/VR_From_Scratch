// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "VRHandAnimationInstance.generated.h"

/**
 * 
 */
UCLASS()
class VRCHARACTER_API UVRHandAnimationInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pose")
	float PoseAlphaTeleport = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pose")
	float IsInDefaultPose = 1;
};
