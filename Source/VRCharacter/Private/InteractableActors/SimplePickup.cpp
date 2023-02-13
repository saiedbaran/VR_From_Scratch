// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActors/SimplePickup.h"


// Sets default values
ASimplePickup::ASimplePickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASimplePickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASimplePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASimplePickup::GrabPressed(AHandSkeletalActor* Hand)
{
	IInteractable::GrabPressed(Hand);

	this->AttachToActor(Cast<AActor>(Hand), FAttachmentTransformRules::KeepWorldTransform);
}

void ASimplePickup::GrabReleased(AHandSkeletalActor* Hand)
{
	IInteractable::GrabReleased(Hand);

	this->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

