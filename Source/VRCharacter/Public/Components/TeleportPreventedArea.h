﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TeleportPreventedArea.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VRCHARACTER_API UTeleportPreventedArea : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTeleportPreventedArea();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

};
