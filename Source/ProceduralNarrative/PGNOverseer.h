// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Graph.h"
#include "PGNUtilities.h"
#include "GameFramework/Actor.h"
#include "PGNOverseer.generated.h"

class UMoodGraph;
class UPGNCharacterDataAsset;
class UPGNEventDataAsset;
class UPGNMoodGraphDataAsset;

UCLASS()
class PROCEDURALNARRATIVE_API APGNOverseer : public AActor
{
	GENERATED_BODY()

	static constexpr float HOW_LONG_BEFORE_GENERATING_NEW_NARRATIVES = 1.f;

public:	
	// Sets default values for this actor's properties
	APGNOverseer();

	UPROPERTY(VisibleAnywhere)
	bool bIsInitialized;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPGNEventDataAsset* EventDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Characters")
	UPGNCharacterDataAsset* CharacterDataAsset;

	UPROPERTY()
	FPGNEvent CurrentEvent;

	UPROPERTY()
	FPGNGeneratedNarrative CurrentNarrative;
	
	// We will use this to keep track of the previous narratives.
	UPROPERTY()
	TArray<FPGNGeneratedNarrative> AllPreviouslyGeneratedNarratives;

#pragma region Characters

	UPROPERTY(VisibleAnywhere, Category = "Characters")
	TArray<FPGNCharacter> AllCharacters;

#pragma endregion Characters

#pragma region Conclusions
	
	UPROPERTY()
	TArray<FPGNConclusionEvent> CONCLUSION_AllEvents;
	
	UPROPERTY()
	FPGNConclusionEvent CurrentConclusionEvent;

	static constexpr float WEIGHTING_FOR_RECENCY_IN_GENERATING_CONCLUSIONS = 0.33f;
	static constexpr float WEIGHTING_FOR_MOOD_GRAPH_IN_GENERATING_CONCLUSIONS = 0.67f;

#pragma endregion Conclusions

#pragma region MoodGraph
	
	UPROPERTY()
	UMoodGraph* MoodGraph;

	UPROPERTY(EditAnywhere, Category = "Mood Graph")
	UPGNMoodGraphDataAsset* MoodGraphDataAsset;

	static constexpr int IDEAL_MOOD_GRAPH_DISTANCE_FROM_LAST_CONCLUSION = 3;

	// We use the mood graph to ensure that we do not jump from one tone to another too quickly.
	int EvaluateScoreFromMoodGraphForThisConclusionEvent(FPGNConclusionEvent& ThisConclusionEvent);

#pragma endregion MoodGraph

protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#pragma region Initialization
	
	void InitializeOverseer();
	bool InitializeAllEvents();

#pragma region Characters

	void InitializeAllCharacters();

	void InitializeAllCharacterPopulationIntervalsFromDataAsset() const;
	
	void InitializeThisIndividualCharacter(FPGNCharacterTemplate& ThisCharacterTemplate, FPGNCharacter& Out_ThisCharacter);
	void InitializeThisCharacterGender(FPGNCharacter& Out_ThisCharacter) const;
	void InitializeThisCharacterAge(FPGNCharacter& Out_ThisCharacter) const;
	void InitializeThisCharacterName(FPGNCharacter& Out_ThisCharacter) const;

	void InitializeAllCharacterRelationships();
	
	void InitializeAllCharactersRomanticRelationships();
	EPGNCharacterRomanticRelationship DetermineRelationshipFromRandomPercentage(float GeneratedChance);
	bool FindPotentialRomanticSpouseForThisCharacter(FPGNCharacter& ThisCharacter);

	void InitializeAllCharactersSocialRelationships();

	EPGNCharacterSocialRelationship DetermineSocialRelationshipFromRandomPercentage(float GeneratedChance);

	UPROPERTY(VisibleAnywhere, Category = "Characters")
	TArray<uint16> AllCharactersWaitingForMarriagePartners;

#pragma endregion Characters

	void InitializeMoodGraph();

#pragma endregion Initialization

public:
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	float TimerSinceLastNarrativeGeneration = 0.f;
	
	void GenerateNewNarrative();
	bool HasCurrentNarrativeReachedConvergence();
};
