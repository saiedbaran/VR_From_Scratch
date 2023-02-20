// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimplePickup.h"
#include "GrabbityGrabStaticMesh.generated.h"


class UNiagaraComponent;

UENUM()
enum EGrabbityStage
{
	Default,
	Hovering,
	Transferring,
	Grabbed
};

UCLASS()
class VRCHARACTER_API AGrabbityGrabStaticMesh : public ASimplePickup
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGrabbityGrabStaticMesh();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**
	 * @brief Overriden functions from IInteractable
	 * @return true as the actor is grabbity ready
	 */
	virtual bool IsGrabbityInteractable() override;
	virtual void GrabbityHoverBegin(AHandSkeletalActor* Hand) override;
	virtual void GrabbityHoverEnd(AHandSkeletalActor* Hand) override;
	virtual void GrabbityGrabPressed(AHandSkeletalActor* Hand) override;
	virtual void GrabbityGrabReleased(AHandSkeletalActor* Hand) override;
	virtual void GrabPressed(AHandSkeletalActor* Hand) override;
	virtual void GrabReleased(AHandSkeletalActor* Hand) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	UStaticMeshComponent* StaticMeshComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	UNiagaraComponent* NiagaraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	USkeletalMeshComponent* SkeletalMeshPH;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Grabbity|Properties")
	float GrabbityGrabDuration = 1.0f;

	EGrabbityStage GrabbityStage = EGrabbityStage::Default;

protected:

	float currentGrabbityGrabTime = 0.0f;
	FVector InitialGrabbityGrabLocation;
	FRotator InitialGrabbityGrabRotation;
	

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	AHandSkeletalActor* GrabbityHoverHand = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	AHandSkeletalActor* GrabbityGrabHand = nullptr;

};
