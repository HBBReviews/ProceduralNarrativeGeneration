// Fill out your copyright notice in the Description page of Project Settings.

#include "PGNUtilities.h"
#include "PGNOverseer.h"

void UPGNUtilities::FindBestConclusionEvent(FPGNConclusionEvent& Out_ConclusionEvent, APGNOverseer* ThisOverseer)
{
	TArray<FPGNConclusionEvent> AllPossibleConclusionEvents = ThisOverseer->CONCLUSION_AllEvents;

	// If we have no previous narratives, then we will just return the a random conclusion event.
	if (ThisOverseer->AllPreviouslyGeneratedNarratives.Num() == 0)
	{
		Out_ConclusionEvent = AllPossibleConclusionEvents[FMath::RandRange(0,
			AllPossibleConclusionEvents.Num() - 1)];
		return;
	}
	
	for (FPGNConclusionEvent& ThisConclusionEvent : AllPossibleConclusionEvents)
	{
		UE_LOG(LogTemp, Warning, TEXT("***********************************"));
		ThisConclusionEvent.EvaluatedScore = EvaluateThisPossibleConclusionEvent(ThisConclusionEvent, ThisOverseer);
		UE_LOG(LogTemp, Warning, TEXT("Evaluated Score: %f FOR THE SUBJECT TAG %s"), ThisConclusionEvent.EvaluatedScore,
			*UEnum::GetValueAsString(ThisConclusionEvent.Action));
	}

	// Determine the best conclusion event
	AllPossibleConclusionEvents.Sort([](const FPGNConclusionEvent& A, const FPGNConclusionEvent& B)
	{
		return A.EvaluatedScore > B.EvaluatedScore;
	});

	// Return that event then
	Out_ConclusionEvent = AllPossibleConclusionEvents[0];
}

float UPGNUtilities::EvaluateThisPossibleConclusionEvent(FPGNConclusionEvent& ThisConclusionEvent, APGNOverseer* ThisOverseer)
{
	/* What we will do is evaluate every possible conclusion event based on three main figures:
	 *
	 * 1. Recency or Usage in Previous Narratives - The more recent an event has been used, the less likely it
	 * will be used again.
	 *
	 * 2. Mood Graph - We want to create a natural flow of tones between our generated narratives. We will look at
	 * nearby neighboring tones and determine which is the best. On our grid, we will evaluate our score based on
	 * the closest to the previous tone.
	 */

	/// EVALUATING RECENCY
	if (ThisConclusionEvent.AllNarrativesThisConclusionEventIsIn.Num() > 0)
	{
		const float TotalAppearancesOfEvent = static_cast<float>(ThisConclusionEvent.AllNarrativesThisConclusionEventIsIn.Num());
		const int LastUsageOfThisEvent = ThisConclusionEvent.AllNarrativesThisConclusionEventIsIn[
			ThisConclusionEvent.AllNarrativesThisConclusionEventIsIn.Num() - 1];

		constexpr float WeightingForDensity = 0.67f;
		constexpr float WeightingForLastUsage = 0.5f;

		int RecencySubScore = (TotalAppearancesOfEvent * WeightingForDensity) + (LastUsageOfThisEvent /
			WeightingForLastUsage);

		constexpr float WEIGHTING_FOR_RECENCY = 2.f;
		RecencySubScore *= WEIGHTING_FOR_RECENCY;

		ThisConclusionEvent.RecencySubScore = RecencySubScore;
	}
	else
	{
		ThisConclusionEvent.RecencySubScore = 0;
	}

	ThisConclusionEvent.RecencySubScore = 0;
	
	/// EVALUATING MOODS
	int MoodGraphSubScore = ThisOverseer->EvaluateScoreFromMoodGraphForThisConclusionEvent(ThisConclusionEvent)
		* ThisOverseer->WEIGHTING_FOR_MOOD_GRAPH_IN_GENERATING_CONCLUSIONS;

	ThisConclusionEvent.MoodGraphSubScore = MoodGraphSubScore;
	UE_LOG(LogTemp, Log, TEXT("HEY CHARLIE! THE MOOD GRAPH SUB SCORE IS %f"), ThisConclusionEvent.MoodGraphSubScore);
	
	// The maximum possible score is 100, and the lowest is 0.
	return ThisConclusionEvent.RecencySubScore + ThisConclusionEvent.MoodGraphSubScore;
}

void UPGNUtilities::DEBUG_PrintOutThisConclusionEvent(FPGNConclusionEvent& In_ConclusionEvent)
{
	UE_LOG(LogTemp, Log, TEXT("THIS CONCLUSION EVENT HAD THIS SUBJECT %s %s"), *UEnum::GetValueAsString(
		In_ConclusionEvent.Subject), *UEnum::GetValueAsString(In_ConclusionEvent.Action));
}

void UPGNUtilities::FindBestNextEvent(FPGNEvent& Out_NextEvent, APGNOverseer* ThisOverseer)
{
	// ToDo: Implement using genetic algorithms.
}

void UPGNUtilities::GeneratePossibleCastOfCharactersForThisEvent(FPGNGeneratedNarrative& Out_GeneratedNarrative,
	FPGNEvent& Out_ThisEvent, APGNOverseer* ThisOverseer)
{
	// ToDo: Implement using the already existing cast of characters.
}
