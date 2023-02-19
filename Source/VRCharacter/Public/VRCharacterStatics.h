#pragma once

#include "CoreMinimal.h"

UENUM()
enum EHandType
{
    LeftHand,
    RightHand,
    None
};

#define ECC_Grabbity ECollisionChannel::ECC_GameTraceChannel1