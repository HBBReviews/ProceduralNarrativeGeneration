// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ProceduralNarrative/PGNUtilities.h"
#include "PGNCharacterDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class PROCEDURALNARRATIVE_API UPGNCharacterDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	
	//// CHARACTER PARAMETERS ////////////////////////////
	UPROPERTY(EditAnywhere, Category = "All Characters")
	TArray<FPGNCharacterTemplate> AllCharacterTemplates;

#pragma region Demographics

	//// DEMOGRAPHIC PARAMETERS ////////////////////////////
	UPROPERTY(EditAnywhere, Category = "Demographics", meta = (ClampMin = 0.f, ClampMax = 1.f))
	float PercentageOfMaleCharacters = 0.5042f;

	UPROPERTY(EditAnywhere, Category = "Demographics")
	TArray<FPGNCharacterDemographicParameters> AllDemographicParameters;

#pragma endregion Demographics

#pragma region Marriage
	
	//// MARRIAGE PARAMETERS ////////////////////////////
	UPROPERTY(EditAnywhere, Category = "Marriage", meta = (ClampMin = 0.f, ClampMax = 1.f))
	float PercentageOfCharactersWhoAreMarried = 0.6754f;

	UPROPERTY(EditAnywhere, Category = "Marriage", meta = (ClampMin = 0.f, ClampMax = 1.f))
	float PercentageOfCharactersWhoAreDivorced = 0.1509f;

	UPROPERTY(EditAnywhere, Category = "Marriage", meta = (ClampMin = 0.f, ClampMax = 1.f))
	float PercentageOfCharactersWhoAreWidowed = 0.0358f;

	UPROPERTY(EditAnywhere, Category = "Marriage", meta = (ClampMin = 0.f, ClampMax = 1.f))
	float PercentageOfCharactersWhoAreSingle = 0.4735f;

#pragma endregion Marriage

#pragma region SocialRelationships
	
	//// SOCIAL RELATIONSHIPS PARAMETERS ////////////////////////////
	UPROPERTY(EditAnywhere, Category = "Social Relationships", meta = (ClampMin = 0.f, ClampMax = 1.f))
	float PercentageOfCharactersWhoAreNeighbors = 0.25f;

	UPROPERTY(EditAnywhere, Category = "Social Relationships", meta = (ClampMin = 0.f, ClampMax = 1.f))
	float PercentageOfCharactersWhoAreFamily = 0.15f;

	UPROPERTY(EditAnywhere, Category = "Social Relationships", meta = (ClampMin = 0.f, ClampMax = 1.f))
	float PercentageOfCharactersWhoAreCoworkers = 0.2f;

	UPROPERTY(EditAnywhere, Category = "Social Relationships", meta = (ClampMin = 0.f, ClampMax = 1.f))
	float PercentageOfCharactersWhoAreFriends = 0.2f;

	UPROPERTY(EditAnywhere, Category = "Social Relationships", meta = (ClampMin = 0.f, ClampMax = 1.f))
	float PercentageOfCharactersWhoAreEnemies = 0.2f;

	UPROPERTY(EditAnywhere, Category = "Social Relationships", meta = (ClampMin = 0.f, ClampMax = 1.f))
	int MaximumDistanceBetweenCharactersInSingleEdge = 10;
	
#pragma endregion SocialRelationships
};
