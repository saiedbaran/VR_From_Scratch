// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HandSkeletalActor.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class VRCHARACTER_API IInteractable
{
	GENERATED_BODY()

public:
	virtual void GrabPressed(AHandSkeletalActor* Hand)	{}
	virtual void GrabReleased(AHandSkeletalActor* Hand)	{}

	virtual bool IsGrabbityInteractable() { return false; }
	virtual void GrabbityHoverBegin(AHandSkeletalActor* Hand) {}
	virtual void GrabbityHoverEnd(AHandSkeletalActor* Hand) {}
	virtual void GrabbityGrabPressed(AHandSkeletalActor* Hand) {}
	virtual void GrabbityGrabReleased(AHandSkeletalActor* Hand) {}
};
