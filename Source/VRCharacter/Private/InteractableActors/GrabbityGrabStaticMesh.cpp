// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActors/GrabbityGrabStaticMesh.h"
#include "NiagaraComponent.h"


// Sets default values
AGrabbityGrabStaticMesh::AGrabbityGrabStaticMesh()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->SetCollisionObjectType(ECC_Grabbity);

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponent->SetupAttachment(StaticMeshComponent);
	NiagaraComponent->SetHiddenInGame(true);
}

// Called when the game starts or when spawned
void AGrabbityGrabStaticMesh::BeginPlay()
{
	Super::BeginPlay();
}

bool AGrabbityGrabStaticMesh::IsGrabbityInteractable()
{
	return true;
}

void AGrabbityGrabStaticMesh::GrabbityHoverBegin(AHandSkeletalActor* Hand)
{
	Super::GrabbityHoverBegin(Hand);

	if(GrabbityStage != Default) {return;}
	
	GrabbityStage = Hovering;
	GrabbityHoverHand = Hand;

	NiagaraComponent->SetHiddenInGame(false);
	NiagaraComponent->SetVectorParameter("user.Direction",
										 ((Hand->HandMesh->GetComponentLocation() - GetActorLocation()).GetSafeNormal()));
}

void AGrabbityGrabStaticMesh::GrabbityHoverEnd(AHandSkeletalActor* Hand)
{
	Super::GrabbityHoverEnd(Hand);

	if(GrabbityStage != Hovering) {return;}

	GrabbityStage = Default;
	GrabbityHoverHand = nullptr;

	NiagaraComponent->SetHiddenInGame(true);
}

void AGrabbityGrabStaticMesh::GrabbityGrabPressed(AHandSkeletalActor* Hand)
{
	Super::GrabbityGrabPressed(Hand);

	InitialGrabbityGrabLocation = GetActorLocation();
	StaticMeshComponent->SetSimulatePhysics(false);
	GrabbityGrabHand = Hand;
	
	GrabbityStage = Transferring;
}

void AGrabbityGrabStaticMesh::GrabbityGrabReleased(AHandSkeletalActor* Hand)
{
	Super::GrabbityGrabReleased(Hand);

	if(GrabbityStage != Transferring || GrabbityStage != Grabbed) {return;}

	StaticMeshComponent->SetSimulatePhysics(true);
	currentGrabbityGrabTime = 0.f;
	GrabbityGrabHand = nullptr;

	this->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

void AGrabbityGrabStaticMesh::GrabPressed(AHandSkeletalActor* Hand)
{
	Super::GrabPressed(Hand);
}

void AGrabbityGrabStaticMesh::GrabReleased(AHandSkeletalActor* Hand)
{
	Super::GrabReleased(Hand);
}

// Called every frame
void AGrabbityGrabStaticMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(GrabbityStage == Transferring)
	{
		float ratio = currentGrabbityGrabTime / GrabbityGrabDuration;

		auto handLocation = GrabbityGrabHand->HandMesh->GetComponentLocation();
		auto currentLocation = FMath::Lerp(InitialGrabbityGrabLocation, handLocation, ratio);

		SetActorLocation(currentLocation);
		
		if(ratio > 0.98f)
		{
			currentGrabbityGrabTime = 0.f;
			GrabbityStage = Grabbed;

			this->AttachToActor(Cast<AActor>(GrabbityGrabHand), FAttachmentTransformRules::KeepWorldTransform);
		}
		currentGrabbityGrabTime += DeltaTime;
	}

	if (GrabbityStage==Hovering && GrabbityHoverHand)
	{
		NiagaraComponent->SetVectorParameter("user.Direction",
		                                     ((GrabbityHoverHand->HandMesh->GetComponentLocation() - GetActorLocation()).GetSafeNormal()));
	}
}
