// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralNarrative/Graph.h"
#include "ProceduralNarrative/PGNUtilities.h"
#include "CharacterGraph.generated.h"

USTRUCT()
struct FCharacterGraphVertex
{
	GENERATED_BODY()
	
	int IndexInAllCharactersArray = -1;

	bool operator == (const FCharacterGraphVertex& Other) const
	{
		return IndexInAllCharactersArray == Other.IndexInAllCharactersArray;
	}

	friend uint32 GetTypeHash (const FCharacterGraphVertex& Other)
	{
		return GetTypeHash(Other.IndexInAllCharactersArray);
	}
};

USTRUCT()
struct FCharacterGraphVertexDistance
{
	GENERATED_BODY()

	UPROPERTY()
	FCharacterGraphVertex Vertex;

	UPROPERTY(EditAnywhere)
	int Distance;

	bool operator == (const FCharacterGraphVertexDistance& Other) const
	{
		return Distance == Other.Distance;
	}
	
	friend uint32 GetTypeHash (const FCharacterGraphVertexDistance& Other)
	{
		return GetTypeHash(Other.Distance);
	}

	FCharacterGraphVertex GetVertex() const
	{
		return Vertex;
	}
};

USTRUCT()
struct FCharacterGraphEdge
{
	GENERATED_BODY()

	UPROPERTY()
	FCharacterGraphVertex VertexA;

	UPROPERTY()
	FCharacterGraphVertex VertexB;

	UPROPERTY(EditAnywhere)
	int DistanceBetweenVertices;

	bool operator == (const FCharacterGraphEdge& Other) const
	{
		return DistanceBetweenVertices == Other.DistanceBetweenVertices;
	}
	
	friend uint32 GetTypeHash (const FCharacterGraphEdge& Other)
	{
		return GetTypeHash(Other.DistanceBetweenVertices);
	}
};

/**
 * 
 */
UCLASS()
class PROCEDURALNARRATIVE_API UCharacterGraph : public UGraph
{
	GENERATED_BODY()

public:

	void InitializeCharacterGraphWithErdosRenyi(APGNOverseer* Overseer, float ThresholdForEdgeCreation,
		int MaximumDistanceBetweenVertices = 1);

	void GenerateListOfVerticesFromOverseer(APGNOverseer* Overseer, TArray<FCharacterGraphVertex>& Out_AllVertices);

	TMap<FCharacterGraphVertex, TArray<FCharacterGraphVertexDistance>> CharacterAdjacencyList;

	TArray<FCharacterGraphEdge> AllEdges;
};
