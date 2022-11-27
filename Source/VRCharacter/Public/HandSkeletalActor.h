// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HandSkeletalActor.generated.h"

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
	void UpdateTeleportPose(float Alpha);
	
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
};
