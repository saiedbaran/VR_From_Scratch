// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimplePickup.h"
#include "SimplePickupStaticMesh.generated.h"

UCLASS()
class VRCHARACTER_API ASimplePickupStaticMesh : public ASimplePickup
{
	GENERATED_BODY()

public:
	ASimplePickupStaticMesh();
	virtual void Tick(float DeltaTime) override;

	/**
	 * @brief Grab event
	 * @param Hand Interacting hand	
	 */
	virtual void GrabPressed(AHandSkeletalActor* Hand) override;
	virtual void GrabReleased(AHandSkeletalActor* Hand) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	UStaticMeshComponent* StaticMeshComponent;
};
