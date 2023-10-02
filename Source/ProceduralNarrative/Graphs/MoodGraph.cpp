// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralNarrative/Graphs/MoodGraph.h"

void UMoodGraph::InitializeMoodGraphWithAllData(TArray<FMoodGraphVertex> AllVertices, TArray<FMoodGraphEdge> AllEdges)
{
	// Initialize all of our vertices
	Vertices = TSet<FMoodGraphVertex>();
	for (FMoodGraphVertex& ThisVertex : AllVertices)
	{
		Vertices.Add(ThisVertex);
	}

	// Initialize all of our edges
	Edges = TSet<FMoodGraphEdge>();
	for (FMoodGraphEdge& ThisEdge : AllEdges)
	{
		ThisEdge.VertexA.Mood = ThisEdge.VertexA_Mood;
		ThisEdge.VertexB.Mood = ThisEdge.VertexB_Mood;
		Edges.Add(ThisEdge);
	}

	// Create an adjacency list.
	AdjacencyList = TMap<FMoodGraphVertex, TArray<FMoodGraphVertexDistance>>();
	for (FMoodGraphVertex& ThisVertex : AllVertices)
	{
		AdjacencyList.Add(ThisVertex, TArray<FMoodGraphVertexDistance>());
	}

	for (FMoodGraphEdge& ThisEdge : AllEdges)
	{
		if (AdjacencyList.Contains(ThisEdge.VertexA))
		{
			FMoodGraphVertexDistance ThisDistance;
			ThisDistance.Vertex = ThisEdge.VertexB;
			ThisDistance.Distance = ThisEdge.DistanceBetweenVertices;
			
			AdjacencyList[ThisEdge.VertexA].Add(ThisDistance);
		}

		if (AdjacencyList.Contains(ThisEdge.VertexB))
		{
			FMoodGraphVertexDistance ThisDistance;
			ThisDistance.Vertex = ThisEdge.VertexA;
			ThisDistance.Distance = ThisEdge.DistanceBetweenVertices;
			AdjacencyList[ThisEdge.VertexB].Add(ThisDistance);
		}
	}
}

TMap<FMoodGraphVertex, int> UMoodGraph::CreateDistanceMapUsingDijkstra(FMoodGraphVertex& StartVertex)
{
	TSet<FMoodGraphVertex> VisitedSet = TSet<FMoodGraphVertex>();
	TMap<FMoodGraphVertex, int> GeneratedDistanceMap = TMap<FMoodGraphVertex, int>();

	// Create a queue of vertices to visit
	TArray<FMoodGraphVertexDistance> Queue = TArray<FMoodGraphVertexDistance>();

	// Add all of our vertices and initialize the distance map to be the maximum value
	for (FMoodGraphVertex ThisVertex : Vertices)
	{
		GeneratedDistanceMap.Add(ThisVertex, MAX_int32);
	}

	FMoodGraphVertexDistance StartVertexDistance;
	StartVertexDistance.Vertex = StartVertex;
	StartVertexDistance.Distance = 0;
	
	Queue.Add(StartVertexDistance);

	while (Queue.Num() > 0 && VisitedSet.Num() < Vertices.Num())
	{
		// Get the next vertex to visit
		FMoodGraphVertexDistance ThisVertexDistance = Queue[0];
		Queue.RemoveAt(0);

		if (!VisitedSet.Contains(ThisVertexDistance.Vertex))
		{
			VisitedSet.Add(ThisVertexDistance.Vertex);

			if (ThisVertexDistance.Distance < GeneratedDistanceMap[ThisVertexDistance.Vertex])
			{
				GeneratedDistanceMap[ThisVertexDistance.Vertex] = ThisVertexDistance.Distance;
			}
			
			for (FMoodGraphVertexDistance ThisDistance : AdjacencyList[ThisVertexDistance.Vertex])
			{
				FMoodGraphVertexDistance NewVertexDistance;
				NewVertexDistance.GetVertex() = ThisDistance.GetVertex();
				NewVertexDistance.Distance = ThisDistance.Distance + ThisVertexDistance.Distance;

				Queue.Add(NewVertexDistance);
			}

			Queue.Sort([](const FMoodGraphVertexDistance& A, const FMoodGraphVertexDistance& B)
			{
				return A.Distance < B.Distance;
			});
		}
	}

	return GeneratedDistanceMap;
}
