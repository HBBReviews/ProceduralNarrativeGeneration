// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralNarrative/Graphs/CharacterGraph.h"

#include "ProceduralNarrative/PGNOverseer.h"

void UCharacterGraph::InitializeCharacterGraphWithErdosRenyi(APGNOverseer* Overseer, float ThresholdForEdgeCreation,
	int MaximumDistanceBetweenVertices)
{
	// Create an adjacency list.
	CharacterAdjacencyList = TMap<FCharacterGraphVertex, TArray<FCharacterGraphVertexDistance>>();
	
	// First, generate all of our vertices from the Overseer.
	TArray<FCharacterGraphVertex> AllVertices;
	GenerateListOfVerticesFromOverseer(Overseer, AllVertices);

	const int NumberOfVertices = AllVertices.Num();

	// These represent our A Vertex.
	for (int i = 0; i < NumberOfVertices; i++)
	{
		// These represent our B vertex.
		// Determine if we should add an edge between this vertex and any other vertex.
		for (int j = 0; j < NumberOfVertices; j++)
		{
			if (AllVertices[j].IndexInAllCharactersArray == AllVertices[i].IndexInAllCharactersArray)
			{
				continue;
			}
			
			// Before anything else, we need to check if this vertex is already has an edge or connection to the other vertex.
			// If it does, we don't need to add another edge.

			bool bShouldContinue = false;
			
			for (auto ThisVertex : CharacterAdjacencyList[AllVertices[j]])
			{
				if (ThisVertex.Vertex == AllVertices[i])
				{
					bShouldContinue = true;
					break;
				}
			}

			for (auto ThisVertex : CharacterAdjacencyList[AllVertices[i]])
			{
				if (ThisVertex.Vertex == AllVertices[j])
				{
					bShouldContinue = true;
					break;
				}
			}

			if (bShouldContinue)
			{
				continue;
			}

			// First, generate our edge probability.
			const float ProbabilityOfEdgeCreation = FMath::FRandRange(0.0f, 1.0f);
			
			if (ProbabilityOfEdgeCreation < ThresholdForEdgeCreation)
			{
				// Add an edge between these two vertices.
				FCharacterGraphEdge ThisEdge;
				ThisEdge.VertexA = AllVertices[i];
				ThisEdge.VertexB = AllVertices[j];
				
				FCharacterGraphVertexDistance ThisDistance;
				ThisDistance.Vertex.IndexInAllCharactersArray = AllVertices[j].IndexInAllCharactersArray;

				// The distance should be calculated based off of how close the probability of edge is to the global threshold.
				constexpr int MinimumDistanceBetweenEdges = 1;
				
				// The lower the generated score, the lower the distance.
				const float AlphaForDistance = (ProbabilityOfEdgeCreation / ThresholdForEdgeCreation);
				
				ThisDistance.Distance = FMath::Clamp(static_cast<int>(AlphaForDistance * MaximumDistanceBetweenVertices),
				                                     MinimumDistanceBetweenEdges, MaximumDistanceBetweenVertices);

				ThisEdge.DistanceBetweenVertices = FMath::Clamp(static_cast<int>(AlphaForDistance * MaximumDistanceBetweenVertices),
													 MinimumDistanceBetweenEdges, MaximumDistanceBetweenVertices);
				AllEdges.Add(ThisEdge);
				
				CharacterAdjacencyList[AllVertices[i]].Add(ThisDistance);
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("*** WE GENERATED %d SOCIAL RELATIONSHIPS FOR OUR CHARACTERS. ***"), AllEdges.Num());
}

void UCharacterGraph::GenerateListOfVerticesFromOverseer(APGNOverseer* Overseer,
	TArray<FCharacterGraphVertex>& Out_AllVertices)
{
	Out_AllVertices.Reserve(Overseer->AllCharacters.Num());

	for (int Index_Character = 0; Index_Character < Overseer->AllCharacters.Num(); Index_Character++)
	{
		FCharacterGraphVertex ThisVertex;
		ThisVertex.IndexInAllCharactersArray = Index_Character;
		Out_AllVertices.Add(ThisVertex);

		CharacterAdjacencyList.Add(ThisVertex, TArray<FCharacterGraphVertexDistance>());
	}
}
