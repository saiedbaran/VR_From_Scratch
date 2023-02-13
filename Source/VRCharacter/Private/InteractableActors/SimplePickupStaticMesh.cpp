// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActors/SimplePickupStaticMesh.h"


// Sets default values
ASimplePickupStaticMesh::ASimplePickupStaticMesh()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	StaticMeshComponent->SetSimulatePhysics(true);
}

// Called when the game starts or when spawned
void ASimplePickupStaticMesh::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASimplePickupStaticMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASimplePickupStaticMesh::GrabPressed(AHandSkeletalActor* Hand)
{
	ASimplePickup::GrabPressed(Hand);

	StaticMeshComponent->SetSimulatePhysics(false);
}

void ASimplePickupStaticMesh::GrabReleased(AHandSkeletalActor* Hand)
{
	ASimplePickup::GrabReleased(Hand);

	StaticMeshComponent->SetSimulatePhysics(true);
}

