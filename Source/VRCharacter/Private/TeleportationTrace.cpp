// Fill out your copyright notice in the Description page of Project Settings.


#include "TeleportationTrace.h"

#include "NiagaraComponent.h"
#include "Components/SplineComponent.h"


// Sets default values
ATeleportationTrace::ATeleportationTrace()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	SplineComponent->SetupAttachment(RootComponent);

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponent->SetupAttachment(RootComponent);
	NiagaraComponent->SetHiddenInGame(true);
}

void ATeleportationTrace::UpdateSplineLocationTangent(FVector BeginLocation, FVector EndLocation, FVector BeginTangent,
	FVector EndTangent, float TangentMultiplier)
{
	SplineComponent->SetLocationAtSplinePoint(0, BeginLocation, ESplineCoordinateSpace::World);
	SplineComponent->SetLocationAtSplinePoint(1, EndLocation, ESplineCoordinateSpace::World);
	SplineComponent->SetTangentAtSplinePoint(0, BeginTangent * TangentMultiplier, ESplineCoordinateSpace::World);
	SplineComponent->SetTangentAtSplinePoint(1, EndTangent * -1.f * TangentMultiplier, ESplineCoordinateSpace::World);
}


// Called when the game starts or when spawned
void ATeleportationTrace::BeginPlay()
{
	Super::BeginPlay();
	
}

