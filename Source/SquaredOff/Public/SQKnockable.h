// SquaredOff - TODO: Write notice

#pragma once
#include "SQKnockable.generated.h"


UINTERFACE(MinimalAPI)
class USQKnockable : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class SQUAREDOFF_API ISQKnockable
{
	GENERATED_IINTERFACE_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Knockback")
	bool AttemptKnock(FVector force);
};
