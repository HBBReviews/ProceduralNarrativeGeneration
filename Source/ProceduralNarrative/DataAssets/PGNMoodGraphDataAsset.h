// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ProceduralNarrative/Graph.h"
#include "ProceduralNarrative/Graphs/MoodGraph.h"
#include "PGNMoodGraphDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class PROCEDURALNARRATIVE_API UPGNMoodGraphDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Vertices")
	TArray<FMoodGraphVertex> AllVertices;

	UPROPERTY(EditAnywhere, Category = "Edges")
	TArray<FMoodGraphEdge> AllEdges;
};
