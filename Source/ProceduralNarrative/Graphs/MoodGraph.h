// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralNarrative/Graph.h"
#include "ProceduralNarrative/PGNUtilities.h"
#include "MoodGraph.generated.h"

USTRUCT()
struct FMoodGraphVertex
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EPGNMood Mood;

	bool operator == (const FMoodGraphVertex& Other) const
	{
		return Mood == Other.Mood;
	}
	
	friend uint32 GetTypeHash (const FMoodGraphVertex& Other)
	{
		return GetTypeHash(Other.Mood);
	}
};

USTRUCT()
struct FMoodGraphVertexDistance
{
	GENERATED_BODY()

	UPROPERTY()
	FMoodGraphVertex Vertex;

	UPROPERTY(EditAnywhere)
	int Distance;

	bool operator == (const FMoodGraphVertexDistance& Other) const
	{
		return Vertex.Mood == Other.Vertex.Mood
			&& Distance == Other.Distance;
	}
	
	friend uint32 GetTypeHash (const FMoodGraphVertexDistance& Other)
	{
		return GetTypeHash(Other.Vertex.Mood);
	}

	FMoodGraphVertex GetVertex() const
	{
		return Vertex;
	}
};

USTRUCT()
struct FMoodGraphEdge
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EPGNMood VertexA_Mood = EPGNMood::MOOD_Depressing;

	UPROPERTY(EditAnywhere)
	EPGNMood VertexB_Mood = EPGNMood::MOOD_Depressing;

	UPROPERTY()
	FMoodGraphVertex VertexA;

	UPROPERTY()
	FMoodGraphVertex VertexB;

	UPROPERTY(EditAnywhere)
	int DistanceBetweenVertices;

	bool operator == (const FMoodGraphEdge& Other) const
	{
		return DistanceBetweenVertices == Other.DistanceBetweenVertices && VertexA_Mood == Other.VertexA_Mood
			&& VertexB_Mood == Other.VertexB_Mood;
	}
	
	friend uint32 GetTypeHash (const FMoodGraphEdge& Other)
	{
		return GetTypeHash(Other.DistanceBetweenVertices);
	}
};

/**
 * 
 */
UCLASS()
class PROCEDURALNARRATIVE_API UMoodGraph : public UGraph
{
	GENERATED_BODY()

public:

	void InitializeMoodGraphWithAllData(TArray<FMoodGraphVertex> AllVertices, TArray<FMoodGraphEdge> AllEdges);

	virtual TMap<FMoodGraphVertex, int> CreateDistanceMapUsingDijkstra(FMoodGraphVertex& StartVertex);
	
	TSet<FMoodGraphVertex> Vertices;
	TSet<FMoodGraphEdge> Edges;

	// Map<Vertex<T>, List<VertexDistance<T>>> adjList;
	TMap<FMoodGraphVertex, TArray<FMoodGraphVertexDistance>> AdjacencyList;

};
