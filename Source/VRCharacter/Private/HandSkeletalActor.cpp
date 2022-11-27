// Fill out your copyright notice in the Description page of Project Settings.


#include "HandSkeletalActor.h"

#include "VRHandAnimationInstance.h"


// Sets default values
AHandSkeletalActor::AHandSkeletalActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Base = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	Base->SetupAttachment(RootComponent);

	HandMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HandMesh"));
	HandMesh->SetupAttachment(Base);
}

// Called when the game starts or when spawned
void AHandSkeletalActor::BeginPlay()
{
	Super::BeginPlay();

	HandAnimationInstance = Cast<UVRHandAnimationInstance>(HandMesh->GetAnimInstance());
	if(HandAnimationInstance)
	{
		HandAnimationInstance->IsInDefaultPose = 0;
	}
}

// Called every frame
void AHandSkeletalActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHandSkeletalActor::UpdateTeleportPose(float Alpha)
{
	if(!HandAnimationInstance) { return; }
	HandAnimationInstance->PoseAlphaTeleport = Alpha;
}

