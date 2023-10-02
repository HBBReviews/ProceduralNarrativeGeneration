// Fill out your copyright notice in the Description page of Project Settings.


#include "PGNOverseer.h"

#include "DataAssets/PGNCharacterDataAsset.h"
#include "DataAssets/PGNEventDataAsset.h"
#include "DataAssets/PGNMoodGraphDataAsset.h"
#include "Graphs/CharacterGraph.h"
#include "Graphs/MoodGraph.h"

// Sets default values
APGNOverseer::APGNOverseer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APGNOverseer::BeginPlay()
{
	Super::BeginPlay();
	InitializeOverseer();
}

void APGNOverseer::InitializeOverseer()
{
	// Characters should be initialized first
	InitializeAllCharacters();
	InitializeAllEvents();
	InitializeMoodGraph();
}

bool APGNOverseer::InitializeAllEvents()
{
	// We are going to get all of the possible conclusion events from our Event Data Asset.
	CONCLUSION_AllEvents = EventDataAsset->AllConclusionEvents;

	if (CONCLUSION_AllEvents.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("There are no conclusion events in the Event Data Asset. Please add some and try again."));
		return false;
	}
	
	return true;
}

void APGNOverseer::InitializeMoodGraph()
{
	MoodGraph = NewObject<UMoodGraph>(this);

	const TArray<FMoodGraphVertex> AllVertices = MoodGraphDataAsset->AllVertices;
	const TArray<FMoodGraphEdge> AllEdges = MoodGraphDataAsset->AllEdges;
	
	MoodGraph->InitializeMoodGraphWithAllData(MoodGraphDataAsset->AllVertices, MoodGraphDataAsset->AllEdges);
}

// Called every frame
void APGNOverseer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// We will generate a new narrative every second.
	TimerSinceLastNarrativeGeneration += DeltaTime;
	if (TimerSinceLastNarrativeGeneration > HOW_LONG_BEFORE_GENERATING_NEW_NARRATIVES)
	{
		TimerSinceLastNarrativeGeneration = 0.f;
		GenerateNewNarrative();
	}
}

void APGNOverseer::InitializeAllCharacters()
{
	InitializeAllCharacterPopulationIntervalsFromDataAsset();
	
	// How many characters should we generate? This should be dictated by the length of the campaign's.
	constexpr int NUM_CHARACTERS_TO_GENERATE = 20;
	AllCharacters.Reserve(NUM_CHARACTERS_TO_GENERATE);
	
	TArray<FPGNCharacterTemplate>& AllCharacterTemplates = CharacterDataAsset->AllCharacterTemplates;

	// Initialize the structs for all of our characters from the given Character Templates.
	for (int Index_Character = 0; Index_Character < NUM_CHARACTERS_TO_GENERATE; Index_Character++)
	{
		// We will first sort our array of all of the character templates based off of the number of times the
		// template has been used thus far.
		AllCharacterTemplates.Sort([](const FPGNCharacterTemplate& A, const FPGNCharacterTemplate& B)
		{
			return A.NumberOfTimesTemplateUsed < B.NumberOfTimesTemplateUsed;
		});

		FPGNCharacterTemplate& ThisCharacterTemplateToUse = AllCharacterTemplates[0];

		FPGNCharacter ThisInitializedCharacter;
		InitializeThisIndividualCharacter(ThisCharacterTemplateToUse, ThisInitializedCharacter);
		
		AllCharacters.Add(ThisInitializedCharacter);
		
		ThisCharacterTemplateToUse.NumberOfTimesTemplateUsed++;
	}

	InitializeAllCharacterRelationships();

	// Debug the results for all of our characters.
	for (FPGNCharacter& DEBUG_ThisCharacter : AllCharacters)
	{
		UE_LOG(LogTemp, Error, TEXT("NAME: %s, AGE: %d, GENERATION: %s, ROMANCE STATUS: %s, ROMANCE PARTNER: %s, IS MALE: %s"),
			*DEBUG_ThisCharacter.Name, DEBUG_ThisCharacter.Age, *UEnum::GetValueAsString(DEBUG_ThisCharacter.Generation),
			*UEnum::GetValueAsString(DEBUG_ThisCharacter.MyRomanticData.RomanticRelationship),
			*DEBUG_ThisCharacter.MyRomanticData.NameOfRomanticPartner, DEBUG_ThisCharacter.bIsMale ? TEXT("TRUE") : TEXT("FALSE"));

		UE_LOG(LogTemp, Log, TEXT("**** ALL SOCIAL RELATIONSHIPS ****"));

		for (auto ThisRelationship : DEBUG_ThisCharacter.AllMySocialData)
		{
			UE_LOG(LogTemp, Log, TEXT("NAME: %s, RELATIONSHIP: %s"), *ThisRelationship.NameOfSocialPartner,
				*UEnum::GetValueAsString(ThisRelationship.SocialRelationship));
		}
	}

	UE_LOG(LogTemp, Log, TEXT("**********************************"));
}

void APGNOverseer::InitializeAllCharacterPopulationIntervalsFromDataAsset() const
{
	int Start = 0.f;

	for (auto ThisDemographic : CharacterDataAsset->AllDemographicParameters)
	{
		ThisDemographic.IntervalOfPopulation = Start + ThisDemographic.PopulationShare;
		Start = ThisDemographic.IntervalOfPopulation;
	}
}

void APGNOverseer::InitializeThisIndividualCharacter(FPGNCharacterTemplate& ThisCharacterTemplate,
	FPGNCharacter& Out_ThisCharacter)
{
	// We will first generate the character's gender.
	InitializeThisCharacterGender(Out_ThisCharacter);

	// We will then generate the character's age as a random number between the min and max age.
	InitializeThisCharacterAge(Out_ThisCharacter);

	InitializeThisCharacterName(Out_ThisCharacter);
}

void APGNOverseer::InitializeThisCharacterGender(FPGNCharacter& Out_ThisCharacter) const
{
	// Generating gender is a very simple process. We will just generate a random number between 0 and 1.
	Out_ThisCharacter.bIsMale = FMath::FRandRange(0.f, 1.f) < CharacterDataAsset->PercentageOfMaleCharacters;
}

void APGNOverseer::InitializeThisCharacterAge(FPGNCharacter& Out_ThisCharacter) const
{
	const float RandomNumber = FMath::FRandRange(0.f, 1.f);

	FPGNCharacterDemographicParameters SelectedDemographic;
	SelectedDemographic.Generation = EPGNCharacterGeneration::GENERATION_Z;
	
	float CurrentMin = 0.f;

	for (FPGNCharacterDemographicParameters ThisDemographic : CharacterDataAsset->AllDemographicParameters)
	{
		const float CurrentMax = CurrentMin + ThisDemographic.PopulationShare;

		if (RandomNumber >= CurrentMin && RandomNumber < CurrentMax)
		{
			SelectedDemographic = ThisDemographic;
			break;
		}
		
		CurrentMin = CurrentMax;
	}

	Out_ThisCharacter.Generation = SelectedDemographic.Generation;
	Out_ThisCharacter.Age = FMath::RandRange(SelectedDemographic.MinimumAge, SelectedDemographic.MaximumAge);

	// If this is Gen Z and our age was invalid, then we will correctly generate it here.
	if (Out_ThisCharacter.Age == 0 && Out_ThisCharacter.Generation == EPGNCharacterGeneration::GENERATION_Z)
	{
		// Then, just generate the age under the presumption that it is Generation Z.
		const FPGNCharacterDemographicParameters GenerationZDemographic = CharacterDataAsset->AllDemographicParameters[
			CharacterDataAsset->AllDemographicParameters.Num() - 1];
		Out_ThisCharacter.Age = FMath::RandRange(GenerationZDemographic.MinimumAge, GenerationZDemographic.MaximumAge);
	}
}

void APGNOverseer::InitializeThisCharacterName(FPGNCharacter& Out_ThisCharacter) const
{
	// We will generate our name based off of the generation of the character.
	TArray<FString> PoolOfPossibleNames;
	for (FPGNCharacterDemographicParameters ThisDemographic : CharacterDataAsset->AllDemographicParameters)
	{
		if (ThisDemographic.Generation == Out_ThisCharacter.Generation)
		{
			PoolOfPossibleNames = Out_ThisCharacter.bIsMale ? ThisDemographic.AllPossibleMaleNames : ThisDemographic.AllPossibleFemaleNames;
		}
	}

	Out_ThisCharacter.Name = PoolOfPossibleNames[FMath::RandRange(0, PoolOfPossibleNames.Num() - 1)];
}

void APGNOverseer::InitializeAllCharacterRelationships()
{
	InitializeAllCharactersRomanticRelationships();
	InitializeAllCharactersSocialRelationships();
}

void APGNOverseer::InitializeAllCharactersRomanticRelationships()
{
	// Clear this from all previously generated narratives.
	AllCharactersWaitingForMarriagePartners.Empty();
	
	// First, determine which characters will be in a marriage relationship.
	for (int i = 0; i < AllCharacters.Num(); i++)
	{
		FPGNCharacter& ThisCharacter = AllCharacters[i];
		
		ThisCharacter.MyRomanticData.RomanticRelationship = DetermineRelationshipFromRandomPercentage(
			FMath::FRandRange(0.f, 1.f));
	
		// This means that this character needs to find a marriage partner.
		if (ThisCharacter.MyRomanticData.RomanticRelationship == EPGNCharacterRomanticRelationship::MARRIED)
		{
			AllCharactersWaitingForMarriagePartners.Add(i);
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("THE NUMBER OF CHARACTERS WAITING FOR PARTNERS IS %d"), AllCharactersWaitingForMarriagePartners.Num());

	for (int i = 0; i < AllCharactersWaitingForMarriagePartners.Num(); i++)
	{
		FPGNCharacter& ThisCharacter = AllCharacters[AllCharactersWaitingForMarriagePartners[i]];

		if (ThisCharacter.bFoundValidMarriagePartner)
		{
			continue;
		}

		// If we cannot find a spouse for this character, then set them to be single.
		if (!FindPotentialRomanticSpouseForThisCharacter(ThisCharacter))
		{
			ThisCharacter.MyRomanticData.RomanticRelationship = EPGNCharacterRomanticRelationship::SINGLE;
			ThisCharacter.bFoundValidMarriagePartner = false;
		}
	}
}

EPGNCharacterRomanticRelationship APGNOverseer::DetermineRelationshipFromRandomPercentage(float GeneratedChance)
{
	GeneratedChance = FMath::Clamp(GeneratedChance, 0.f, 1.f);
	if (GeneratedChance <= CharacterDataAsset->PercentageOfCharactersWhoAreMarried)
	{
		return EPGNCharacterRomanticRelationship::MARRIED;
	}

	float CurrentIntervalLocation = CharacterDataAsset->PercentageOfCharactersWhoAreMarried;

	if (GeneratedChance <= CharacterDataAsset->PercentageOfCharactersWhoAreDivorced + CurrentIntervalLocation)
	{
		return EPGNCharacterRomanticRelationship::DIVORCED;
	}

	CurrentIntervalLocation += CharacterDataAsset->PercentageOfCharactersWhoAreDivorced;

	if (GeneratedChance <= CharacterDataAsset->PercentageOfCharactersWhoAreWidowed + CurrentIntervalLocation)
	{
		return EPGNCharacterRomanticRelationship::WIDOWED;
	}

	return EPGNCharacterRomanticRelationship::SINGLE;
}

bool APGNOverseer::FindPotentialRomanticSpouseForThisCharacter(FPGNCharacter& ThisCharacter)
{
	// We are going to search through all of the characters and find a potential romantic partner for this character.
	for (int Index_Character = 0; Index_Character < AllCharactersWaitingForMarriagePartners.Num(); Index_Character++)
	{
		FPGNCharacter& ThisPotentialRomanticPartner = AllCharacters[AllCharactersWaitingForMarriagePartners[Index_Character]];
		if (ThisPotentialRomanticPartner.bFoundValidMarriagePartner)
		{
			continue;
		}

		// This means that this character will be married and is of the opposite gender.
		const bool bCanThisCharacterBeASuitor =
			ThisPotentialRomanticPartner.MyRomanticData.RomanticRelationship == EPGNCharacterRomanticRelationship::MARRIED
			&& ThisPotentialRomanticPartner.Generation == ThisCharacter.Generation
			&& ThisPotentialRomanticPartner.bIsMale != ThisCharacter.bIsMale;
		
		if (bCanThisCharacterBeASuitor)
		{
			ThisPotentialRomanticPartner.MyRomanticData.NameOfRomanticPartner = ThisCharacter.Name;
			ThisCharacter.MyRomanticData.NameOfRomanticPartner = ThisPotentialRomanticPartner.Name;
			
			ThisPotentialRomanticPartner.bFoundValidMarriagePartner = true;
			ThisCharacter.bFoundValidMarriagePartner = true;
			
			return true;
		}
	}

	return false;
}

void APGNOverseer::InitializeAllCharactersSocialRelationships()
{
	UE_LOG(LogTemp, Log, TEXT("* INITIALIZING ALL CHARACTERS SOCIAL RELATIONSHIPS *"));

	const float ThresholdForEdgeCreation = FMath::FRandRange(0.f, 1.f);

	UE_LOG(LogTemp, Warning, TEXT("THE PROBABILITY IS %f"), ThresholdForEdgeCreation);

	// This will generate edges randomly between all of our characters.
	UCharacterGraph* CharacterRelationsGraph = NewObject<UCharacterGraph>(this);
	CharacterRelationsGraph->InitializeCharacterGraphWithErdosRenyi(this, ThresholdForEdgeCreation,
		CharacterDataAsset->MaximumDistanceBetweenCharactersInSingleEdge);

	// However, we still need to go through and determine the distances/weights of all the characters.
	// We need to use the parameters as defined by the Character data asset to determine the social relationship.

	/* All Possible Social Relationships:
	 *
	 * 1. Neighbor
	 * 2. Family
	 * 3. Coworker
	 * 4. Friend
	 * 5. Enemy
	 */

	// We will then take all of our created edges in the graph, and then determine the social relationship between the two characters.
	// When we have the graph, we already have the distances. Based off of these distances, we will then use the
	// weights dictated by the data asset.

	// Divide the assigned distance by the maximum distance to get a percentage.

	// After we do this for all edges in the graph, we will have successfully generated all social relationships.

	for (const auto& ThisEdge : CharacterRelationsGraph->AllEdges)
	{
		const float PercentageOfMaxDistance = static_cast<float>(ThisEdge.DistanceBetweenVertices) /
			CharacterDataAsset->MaximumDistanceBetweenCharactersInSingleEdge;

		const EPGNCharacterSocialRelationship SocialRelationship = DetermineSocialRelationshipFromRandomPercentage(
			PercentageOfMaxDistance);

		const int IndexOfCharacterA = ThisEdge.VertexA.IndexInAllCharactersArray;
		const int IndexOfCharacterB = ThisEdge.VertexB.IndexInAllCharactersArray;

		// Generate a new social relationship struct for both characters.
		FPGNCharacterSocialData NewSocialDataForCharacterA;
		NewSocialDataForCharacterA.NameOfSocialPartner = AllCharacters[IndexOfCharacterB].Name;
		NewSocialDataForCharacterA.SocialRelationship = SocialRelationship;

		AllCharacters[IndexOfCharacterA].AllMySocialData.Add(NewSocialDataForCharacterA);

		FPGNCharacterSocialData NewSocialDataForCharacterB;
		NewSocialDataForCharacterB.NameOfSocialPartner = AllCharacters[IndexOfCharacterA].Name;
		NewSocialDataForCharacterB.SocialRelationship = SocialRelationship;

		AllCharacters[IndexOfCharacterB].AllMySocialData.Add(NewSocialDataForCharacterB);
	}
}

EPGNCharacterSocialRelationship APGNOverseer::DetermineSocialRelationshipFromRandomPercentage(float GeneratedChance)
{
	float CurrentStartOfInterval = 0.f;
	float CurrentEndOfInterval = CurrentStartOfInterval + CharacterDataAsset->PercentageOfCharactersWhoAreNeighbors;

	if (GeneratedChance >= CurrentStartOfInterval && GeneratedChance <= CurrentEndOfInterval)
	{
		return EPGNCharacterSocialRelationship::NEIGHBORS;
	}

	CurrentStartOfInterval = CurrentEndOfInterval;
	CurrentEndOfInterval += CharacterDataAsset->PercentageOfCharactersWhoAreFamily;

	if (GeneratedChance >= CurrentStartOfInterval && GeneratedChance <= CurrentEndOfInterval)
	{
		return EPGNCharacterSocialRelationship::FAMILY;
	}

	CurrentStartOfInterval = CurrentEndOfInterval;
	CurrentEndOfInterval += CharacterDataAsset->PercentageOfCharactersWhoAreCoworkers;

	if (GeneratedChance >= CurrentStartOfInterval && GeneratedChance <= CurrentEndOfInterval)
	{
		return EPGNCharacterSocialRelationship::COWORKERS;
	}

	CurrentStartOfInterval = CurrentEndOfInterval;
	CurrentEndOfInterval += CharacterDataAsset->PercentageOfCharactersWhoAreFriends;

	if (GeneratedChance >= CurrentStartOfInterval && GeneratedChance <= CurrentEndOfInterval)
	{
		return EPGNCharacterSocialRelationship::FRIENDS;
	}

	return EPGNCharacterSocialRelationship::ENEMIES;
}

void APGNOverseer::GenerateNewNarrative()
{
	UE_LOG(LogTemp, Error, TEXT("_____________________________________________________"));

	if (CurrentNarrative.bIsNarrativeInitialized)
	{
		AllPreviouslyGeneratedNarratives.Add(CurrentNarrative);
	}

	FPGNGeneratedNarrative NewNarrative;

#pragma region GenerateConclusionEvent
	
	// First, we need to randomly generate a conclusion.
	// ToDo: Later on, we will want to use genetic algorithms to determine the best conclusion event.
	UPGNUtilities::FindBestConclusionEvent(CurrentConclusionEvent, this);
	UPGNUtilities::GeneratePossibleCastOfCharactersForThisEvent(NewNarrative, CurrentConclusionEvent,
		this);

	// Assign this value so we can use it for recency checks later down the line.
	CurrentConclusionEvent.AllNarrativesThisConclusionEventIsIn.Add(AllPreviouslyGeneratedNarratives.Num());
	
	NewNarrative.ConclusionEvent = CurrentConclusionEvent;

	UPGNUtilities::DEBUG_PrintOutThisConclusionEvent(CurrentConclusionEvent);

#pragma endregion GenerateConclusionEvent

#pragma region GenerateEvents

	// Convergence is used for when we have reached the end of our narrative / genetic algorithm.
	while (!HasCurrentNarrativeReachedConvergence())
	{
		UPGNUtilities::FindBestNextEvent(CurrentEvent, this);
		UPGNUtilities::GeneratePossibleCastOfCharactersForThisEvent(NewNarrative, CurrentEvent,
			this);
	}

#pragma endregion GenerateEvents

	NewNarrative.bIsNarrativeInitialized = true;
	
	CurrentNarrative = NewNarrative;

	UE_LOG(LogTemp, Error, TEXT("_____________________________________________________"));
}

bool APGNOverseer::HasCurrentNarrativeReachedConvergence()
{
	// ToDo: This needs to be implemented.
	return true;
}

// We use the mood graph to ensure that we do not jump from one tone to another too quickly.
int APGNOverseer::EvaluateScoreFromMoodGraphForThisConclusionEvent(FPGNConclusionEvent& ThisConclusionEvent)
{
	/* All Moods to Use (12):
	 *
	 * VERTICES:
	 * 1. Joyful
	 * 2. Optimistic
	 * 3. Playful
	 * 4. Empathetic
	 * 5. Sentimental
	 * 6. Serene
	 * 7. Inquisitive
	 * 8. Romantic
	 * 9. Exciting
	 * 10. Disturbing
	 * 11. Depressing
	 * 12. Melancholy
	 */

	// Get the last conclusion event.
	const FPGNConclusionEvent& LastConclusionEvent = AllPreviouslyGeneratedNarratives[
		AllPreviouslyGeneratedNarratives.Num() - 1].ConclusionEvent;

	// We will want to get the three closest conclusion events in absolute distance from the last conclusion event.
	FMoodGraphVertex LastConclusionEventMoodVertex;
	LastConclusionEventMoodVertex.Mood = LastConclusionEvent.Mood;
	
	TMap<FMoodGraphVertex, int> DistanceMapForLastConclusionEvent = MoodGraph->CreateDistanceMapUsingDijkstra(
		LastConclusionEventMoodVertex);

	// Now, we are going to get the distance from the last conclusion event to this potential new conclusion event.
	FMoodGraphVertex ThisEventMoodVertex;
	ThisEventMoodVertex.Mood = ThisConclusionEvent.Mood;
	
	int DistanceFromThisEventToLastConclusionEvent = DistanceMapForLastConclusionEvent[ThisEventMoodVertex];
	
	// By default, we will just try to minimize or incentivize distance.
	DistanceFromThisEventToLastConclusionEvent /= static_cast<float>(IDEAL_MOOD_GRAPH_DISTANCE_FROM_LAST_CONCLUSION);
	
	return DistanceFromThisEventToLastConclusionEvent;
}
