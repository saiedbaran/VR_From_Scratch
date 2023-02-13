// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interactable.h"
#include "SimplePickup.generated.h"

UCLASS()
class VRCHARACTER_API ASimplePickup : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASimplePickup();
	virtual void Tick(float DeltaTime) override;
	
	virtual void GrabPressed(AHandSkeletalActor* Hand) override;
	virtual void GrabReleased(AHandSkeletalActor* Hand) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	
};
