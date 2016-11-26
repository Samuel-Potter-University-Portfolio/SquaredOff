// SquaredOff - TODO: Write notice

#pragma once

#include "GameFramework/WorldSettings.h"
#include "SQWorldSettings.generated.h"

/**
 * 
 */
UCLASS()
class SQUAREDOFF_API ASQWorldSettings : public AWorldSettings
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Level Summary")
	FName display_name = FName(TEXT("Untitled Level"));

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Level Summary")
	FName creator_name = FName(TEXT("Unknown"));

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Level Summary")
	FString description = TEXT("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Duis facilisis sed erat at commodo. Morbi efficitur nulla id risus luctus.");

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Level Summary")
	bool hide_in_menus = false;

	UPROPERTY(BlueprintReadOnly, EditInstanceOnly, Category = "Level Summary")
	UTexture2D* screenshot;
	
};
