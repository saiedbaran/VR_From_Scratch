#pragma once

#include "CoreMinimal.h"
#include "VRCharacterBase.h"
#include "Components/ActorComponent.h"
#include "Teleportation.generated.h"


class ATeleportationTrace;
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class VRCHARACTER_API UTeleportation : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTeleportation();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void BeginTeleport(float ratio);
	
	UFUNCTION(BlueprintCallable)
		void EndTeleport();
	void TraceForTeleportLocation();

	UFUNCTION(BlueprintCallable)
	void TeleportAction();

	UFUNCTION(BlueprintCallable)
	void TeleportCharacter();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:

protected:
	bool bIsTracingForTeleportLocation = false;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = "Components")
	ATeleportationTrace* TeleportTrace;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	AVRCharacterBase* VRCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleportation")
	float TeleportTraceProjectileVelocity = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleportation")
	float TeleportDelay = 0.5f;

private:
	bool bFoundTeleportLocation = true;
	FVector TeleportLocation;
};
