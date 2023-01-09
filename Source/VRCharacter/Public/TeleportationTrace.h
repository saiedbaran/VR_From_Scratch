// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TeleportationTrace.generated.h"

class USplineComponent;
class UNiagaraComponent;

UCLASS()
class VRCHARACTER_API ATeleportationTrace : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATeleportationTrace();

	UFUNCTION(BlueprintCallable)
	void UpdateSplineLocationTangent(FVector BeginLocation, FVector EndLocation,
		FVector BeginTangent, FVector EndTangent,float TangentMultiplier = 180.0f);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USplineComponent* SplineComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UNiagaraComponent* NiagaraComponent; 
};
