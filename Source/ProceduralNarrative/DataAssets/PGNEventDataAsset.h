// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ProceduralNarrative/PGNUtilities.h"
#include "PGNEventDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class PROCEDURALNARRATIVE_API UPGNEventDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	//// NARRATIVE PARAMETERS ////////////////////////////
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Narrative Parameters")
	float DesiredFinalDramaticTension = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "All Events")
	TArray<FPGNEvent> AllNonConclusionEvents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "All Events")
	TArray<FPGNConclusionEvent> AllConclusionEvents;
};
