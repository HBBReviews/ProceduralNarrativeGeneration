// A collection of the necessary enums, structs, and basic functions for generating narratives.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PGNUtilities.generated.h"

class APGNOverseer;

//// ALL ENUMS //////////////////////////////
// We will divide up all sectors into five different areas for better sorting and dramatic tension evaluation.
UENUM(BlueprintType)
enum class EEventSector : uint8
{
	INTRODUCTION,
	INTERMEDIARY_ACTION,
	RISING_ACTION,
	CLIMAX,
	CONCLUSION
};

// Classify each generated event in the Data Asset into one of these categories.
UENUM(BlueprintType)
enum class EPGNMood : uint8
{
	MOOD_Joyful,
	MOOD_Optimistic,
	MOOD_Playful,
	MOOD_Empathetic,
	MOOD_Sentimental,
	MOOD_Serene,
	MOOD_Inquisitive,
	MOOD_Romantic,
	MOOD_Exciting,
	MOOD_Disturbing,
	MOOD_Depressing,
	MOOD_Melancholy
};

#pragma region Characters

// Tag each character so we can easily identify them in code.
UENUM(BlueprintType)
enum class EPGNCharacterTag : uint8
{
	TAG_NONE,
	TAG_PROTAGONIST,
	TAG_CHARACTER_1,
	TAG_CHARACTER_2,
	TAG_CHARACTER_3,
	TAG_CHARACTER_4,
	TAG_CHARACTER_5,
	TAG_CHARACTER_6,
	TAG_CHARACTER_7,
	TAG_CHARACTER_8,
	TAG_CHARACTER_9,
	TAG_CHARACTER_10,
	TAG_CHARACTER_11,
	TAG_CHARACTER_12,
	TAG_CHARACTER_13,
	TAG_CHARACTER_14,
	TAG_CHARACTER_15
};

// These are the different attitudes that a character can have.
UENUM(BlueprintType)
enum class EPGNCharacterAttitude : uint8
{
	ATTITUDE_NONE,
	DEPRESSED,
	JOYFUL,
	AMAZED,
	BOLD,
	COMFORTABLE,
	OPTIMISTIC,
	PROUD,
	AGGRAVATED,
	PESSIMISTIC,
	TENSE,
	ANXIOUS,
	TIRED,
	DISTURBED,
	CURIOUS,
	SHY,
	SENTIMENTAL
};

// These are the different occupations that a character can have.
UENUM(BlueprintType)
enum class EPGNCharacterOccupation : uint8
{
	SOFTWARE_ENGINEER,
	DOCTOR,
	TEACHER,
	PROFESSOR,
	ACCOUNTANT,
	ACTOR,
	ARCHITECT,
	ARTIST,
	AUTHOR,
	BANKER,
	BARBER,
};

// These should be independent of the occupation.
UENUM(BlueprintType)
enum class EPGNCharacterDesire : uint8
{
	RAISE_A_FAMILY,
	ACHIEVE_WEALTH,
	ACHIEVE_FAME,
	ACHIEVE_KNOWLEDGE,
	FIND_LOVE,
	COLLECT_POSSESSIONS,
	BE_RESPECTED
};

// We use this to generate age, demographic, and names.
UENUM(BlueprintType)
enum class EPGNCharacterGeneration : uint8
{
	// 1945 and Before
	SILENT_GENERATION,
	// 1946 to 1964
	BABY_BOOMERS,
	// 1965 to 1980
	GENERATION_X,
	// 1981 to 1996
	MILLENNIALS,
	// 1997 to 2012
	GENERATION_Z
};

UENUM()
enum class EPGNCharacterRomanticRelationship : uint8
{
	MARRIED,
	DIVORCED,
	WIDOWED,
	SINGLE
};

UENUM()
enum class EPGNCharacterSocialRelationship : uint8
{
	NEIGHBORS,
	FAMILY,
	COWORKERS,
	FRIENDS,
	ENEMIES
};

#pragma endregion Characters

UENUM(BlueprintType)
enum class EPGNEventAction : uint8
{
	ACTION_NONE,
	KILLED_IN_HIT_AND_RUN,
	DEPARTS_THE_TOWN,
	DIVORCES_AND_ENDS_MARRIAGE_WITH,
	SENTENCED_TO_PRISON,
	PROMISES_TO_BE_BETTER,
	BECOMES_A_DRUNK,
	LOSES_JOB,
	CAUGHT_ROBBING,
	WINS_LAWSUIT_AGAINST,
	REVEALED_PREGNANT_WITH_CHILD_OF
};

UENUM(BlueprintType)
enum class EPGNEventSetting : uint8
{
	SETTING_NONE,
};

UENUM(BlueprintType)
enum class EPGNEventTime : uint8
{
	TIME_NONE,
	
	// 4 AM to 6 AM
	DAWN,
	// 6 AM to 8 AM
	EARLY_MORNING,
	// 8 AM to 12 PM
	LATE_MORNING,
	// 12 PM to 1 PM
	NOON,
	// 1 PM to 3 PM
	EARLY_AFTERNOON,
	// 3 PM to 5 PM
	LATE_AFTERNOON,
	// 5 PM to 8 PM
	EARLY_NIGHT,
	// 8 PM to 4 AM
	LATE_NIGHT
};

//// ALL STRUCTS ////////////////////////////

#pragma region Characters

USTRUCT(BlueprintType)
struct FPGNCharacterTemplate
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EPGNCharacterOccupation Occupation;

	UPROPERTY(EditAnywhere)
	int OccupationSalary;

	UPROPERTY(EditAnywhere)
	EPGNCharacterDesire CharacterGreatestDesire;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float TendencyTowardsMoralDecisions = 0.5f;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float TendencyToPursueGreatestDesire = 0.75f;

	UPROPERTY(VisibleAnywhere)
	int NumberOfTimesTemplateUsed = 0;
};

USTRUCT()
struct FPGNCharacterRomanticData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	EPGNCharacterRomanticRelationship RomanticRelationship = EPGNCharacterRomanticRelationship::SINGLE;

	UPROPERTY()
	FString NameOfRomanticPartner = "INVALID";

	// ToDo: We really need some kind of ID system.
};

USTRUCT()
struct FPGNCharacterSocialData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	EPGNCharacterSocialRelationship SocialRelationship = EPGNCharacterSocialRelationship::FRIENDS;

	UPROPERTY()
	FString NameOfSocialPartner = "INVALID";
};

// These are instances created from the Character Templates.
USTRUCT(BlueprintType)
struct FPGNCharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	FString Name = "NAME_UNKNOWN";

	UPROPERTY(VisibleAnywhere)
	uint8 Age = 0;

	UPROPERTY(VisibleAnywhere)
	EPGNCharacterGeneration Generation;

	UPROPERTY(VisibleAnywhere)
	FPGNCharacterRomanticData MyRomanticData;

	UPROPERTY(VisibleAnywhere)
	TArray<FPGNCharacterSocialData> AllMySocialData;

	bool bFoundValidMarriagePartner = false;

	// If this is not true, then the character will be female.
	UPROPERTY(VisibleAnywhere)
	bool bIsMale = true;

	UPROPERTY(VisibleAnywhere)
	FPGNCharacterTemplate MyCharacterTemplate;
};

USTRUCT(BlueprintType)
struct FPGNCharacterDemographicParameters
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EPGNCharacterGeneration Generation = EPGNCharacterGeneration::BABY_BOOMERS;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float PopulationShare = 0.f;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	int MinimumAge = 0;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0"))
	int MaximumAge = 0;

	UPROPERTY(EditAnywhere)
	TArray<FString> AllPossibleMaleNames;

	UPROPERTY(EditAnywhere)
	TArray<FString> AllPossibleFemaleNames;

	UPROPERTY()
	float IntervalOfPopulation = 0.f;
};

#pragma endregion Characters

USTRUCT(BlueprintType)
struct FPGNEvent
{
	GENERATED_BODY()

	/* Subject
		There are two possible options for the subject. Firstly, the author of the event can manually set a character
		template that must be used for this event.
		
		Secondly, the author of the event can also set individual parameters on the event that would be used to select
		which character template can fit within this event. However, this is an optional element that should not be
		prioritized.
	Subject - Initial Attitude:
		This should not be the mood of the event.
		
		Instead, this is the attitude of the subject himself or herself.
		
		Remember that the initial state is not guaranteed. Instead, the ideal state that the user selects is simply
		just an ideal case.
	Subject - Final Attitude:
		The final attitude is deterministic.
	Action:
		These should be defined Enums.
		
		Later on, we need to figure out to make this more user-friendly, but this is satisfactory for now.
	Object:
		The object should be entirely optional.
		For now, the object should be of a character template.
		Later on, we may consider making an FPGNObject which could be used in this scenario.
		This FPGNObject should apply to inanimate objects as well as actual characters.
	Object - Initial Attitude:
		Remember that the initial state is not guaranteed. Instead, the ideal state that the user selects is simply just an ideal case.
	Object - Final Attitude:
		The final attitude is deterministic.
	Setting:
		By default, we will ignore the setting constraint and have it evaluated at runtime.
		However, for logical consistency, it may be necessary for an event to have a constrained setting.
		Thus, we will have a list of allowed settings.
	Time of Day:
		By default, we will ignore the time of day constraint.
		However, the user should be able to apply a constraint by having a list of allowed time of days.
		We want this to be a list instead of a single value.

	*/

#pragma region Subject
	
	UPROPERTY(EditAnywhere, Category = "Subject")
	EPGNCharacterTag Subject;

	UPROPERTY(EditAnywhere, Category = "Subject")
	EPGNCharacterAttitude SubjectInitialAttitude;

	UPROPERTY(EditAnywhere, Category = "Subject")
	EPGNCharacterAttitude SubjectFinalAttitude;

#pragma endregion Subject

	UPROPERTY(EditAnywhere, Category = "Action")
	EPGNEventAction Action;

#pragma region Object

	UPROPERTY(EditAnywhere, Category = "Object")
	bool bDoesEventHaveObject = false;

	UPROPERTY(EditAnywhere, Category = "Object", meta = (EditCondition = "bDoesEventHaveObject"))
	EPGNCharacterTag Object;

	UPROPERTY(EditAnywhere, Category = "Object", meta = (EditCondition = "bDoesEventHaveObject"))
	EPGNCharacterAttitude ObjectInitialAttitude;

	UPROPERTY(EditAnywhere, Category = "Object", meta = (EditCondition = "bDoesEventHaveObject"))
	EPGNCharacterAttitude ObjectFinalAttitude;

#pragma endregion Object

#pragma region Setting

	UPROPERTY(EditAnywhere, Category = "Setting")
	bool bDoesEventOverrideSetting = false;

	UPROPERTY(EditAnywhere, Category = "Setting", meta = (EditCondition = "bDoesEventOverrideSetting"))
	EPGNEventSetting Setting;

#pragma endregion Setting

#pragma region Time of Day
	
	UPROPERTY(EditAnywhere, Category = "Time of Day")
	bool bDoesEventOverrideTimeOfDay = false;

	UPROPERTY(EditAnywhere, Category = "Time of Day", meta = (EditCondition = "bDoesEventOverrideTimeOfDay"))
	EPGNEventTime TimeOfDay;

#pragma endregion Time of Day

	UPROPERTY(EditAnywhere, Category = "Mood")
	EPGNMood Mood = EPGNMood::MOOD_Joyful;
	
};

USTRUCT(BlueprintType)
struct FPGNConclusionEvent : public FPGNEvent
{
	GENERATED_BODY()

	float EvaluatedScore = 0;
	
	float RecencySubScore = 0;
	float MoodGraphSubScore = 0;

	UPROPERTY()
	TArray<int> AllNarrativesThisConclusionEventIsIn;
};

USTRUCT(BlueprintType)
struct FPGNGeneratedNarrative
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	bool bIsNarrativeInitialized = false;

	UPROPERTY(VisibleAnywhere)
	FPGNConclusionEvent ConclusionEvent;

	UPROPERTY(VisibleAnywhere)
	TArray<FPGNEvent> AllEvents;

	UPROPERTY(VisibleAnywhere)
	TArray<FPGNCharacterTemplate> AllCharactersInUse;
};

/**
 * 
 */
UCLASS()
class PROCEDURALNARRATIVE_API UPGNUtilities : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	static void FindBestConclusionEvent(FPGNConclusionEvent& Out_ConclusionEvent, APGNOverseer* ThisOverseer);
	static float EvaluateThisPossibleConclusionEvent(FPGNConclusionEvent& ThisConclusionEvent, APGNOverseer* ThisOverseer);

	static void DEBUG_PrintOutThisConclusionEvent(FPGNConclusionEvent& In_ConclusionEvent);

	static void FindBestNextEvent(FPGNEvent& Out_NextEvent, APGNOverseer* ThisOverseer);

	static void GeneratePossibleCastOfCharactersForThisEvent(FPGNGeneratedNarrative& Out_GeneratedNarrative,
		FPGNEvent& Out_ThisEvent, APGNOverseer* ThisOverseer);
};
