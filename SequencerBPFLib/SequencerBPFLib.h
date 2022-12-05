

#pragma once

#include "CoreMinimal.h"
#include "Math/Range.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Evaluation/MovieSceneCompletionMode.h"
#include "Evaluation/Blending/MovieSceneBlendType.h"
#include "Misc/FrameNumber.h"
#include "Math/RangeBound.h"
#include "Misc/QualifiedFrameTime.h"
#include "Misc/Optional.h"
#include "UObject/StructOnScope.h"
#include "SequencerScriptingRange.h"
#include "SequencerBPFLib.generated.h"

class UMovieScene;
class UMovieSceneTrack;
class UMovieSceneNameableTrack;
class UMovieSceneSection;
class UMovieSceneSequence;
class UMovieSceneFolder;
struct FMovieSceneChannel;
struct FMovieSceneChannelEntry;

UCLASS(MinimalAPI, BlueprintType)
class USequencerBPFLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	static UMovieSceneTrack* FindTrackWithGUID(UMovieSceneSequence* Sequencer, TSubclassOf<UMovieSceneTrack> TrackClass, FGuid ObjectGuid, FName TrackName);

	UFUNCTION(BlueprintCallable, meta = (DevelopmentOnly))
	static UMovieSceneTrack* FindTrack(UMovieSceneSequence* Sequencer, TSubclassOf<UMovieSceneTrack> TrackClass, FText TrackName);

	UFUNCTION(BlueprintCallable, meta = (DevelopmentOnly))
	static FGuid FindGUID(UMovieSceneSequence* Sequencer, TSubclassOf<UMovieSceneTrack> TrackClass, FText TrackName);

	UFUNCTION(BlueprintCallable)
	static TArray<UMovieSceneSection*> GetAllSections(UMovieSceneSequence* Sequencer);

	UFUNCTION(BlueprintCallable)
	static void FindFolder(TArray<UMovieSceneFolder*> Folders, FName FolderName, bool& Found, UMovieSceneFolder*& FoundFolder);

};


UCLASS(MinimalAPI, BlueprintType)
class UMovieSceneTrackBPFLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	static FGuid FindObjectBindingGuid(const UMovieSceneTrack* Track);

	UFUNCTION(BlueprintCallable, meta = (DevelopmentOnly))
	static FText GetTrackName(const UMovieSceneNameableTrack* Track);

	UFUNCTION(BlueprintCallable)
	static TArray<UMovieSceneSection*> GetAllSections(const UMovieSceneTrack* Track);

	UFUNCTION(BlueprintCallable)
	static int32 GetSectionsNum(const UMovieSceneTrack* Track);

	UFUNCTION(BlueprintCallable)
	static UMovieSceneSection* GetSection(const UMovieSceneTrack* Track, int32 SectionIndex);

};


UCLASS(MinimalAPI, BlueprintType)
class UMovieSceneSectionBPFLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	static struct FMovieSceneChannel* GetChannel(UMovieSceneSection* Section, int32 EntryIndex, int32 ChannelIndex);

	static struct FMovieSceneChannel* GetFirstChannel(UMovieSceneSection* Section);

	UFUNCTION(BlueprintCallable)
	static int32 GetChannelsNum(UMovieSceneSection* Section);

	UFUNCTION(BlueprintCallable)
	static FName GetFirstChannelTypeName(UMovieSceneSection* Section);

	UFUNCTION(BlueprintCallable)
	static void GetAllChannelTypeNames(UMovieSceneSection* Section, TArray<FName>& EntryChannelTypeNames);

	UFUNCTION(BlueprintCallable)
	static void GetKeyTimes(UMovieSceneSection* Section, FInt32Range WithinRange, TArray<int32>& OutKeyTimes);

	UFUNCTION(BlueprintCallable)
	static void GetChannelKeyTimes(UMovieSceneSection* Section, int32 ChannelIndex, FInt32Range WithinRange, TArray<int32>& OutKeyTimes);

	UFUNCTION(BlueprintCallable)
	void GetAllKeySeconds(UMovieSceneSection* Section, FFrameRate FrameRate, TArray<float>& OutKeySeconds);

	UFUNCTION(BlueprintCallable)
	static void GetKeySeconds(UMovieSceneSection* Section, FSequencerScriptingRange WithinRange, FFrameRate FrameRate, TArray<float>& OutKeySeconds);

	UFUNCTION(BlueprintCallable)
	static bool IsReadOnly(const UMovieSceneSection* Section);

	UFUNCTION(BlueprintCallable)
	static FSequencerScriptingRange GetRange(const UMovieSceneSection* Section, FFrameRate FrameRate);

	UFUNCTION(BlueprintCallable)
	static FSequencerScriptingRange GetTrueRange(const UMovieSceneSection* Section, FFrameRate FrameRate);

	UFUNCTION(BlueprintCallable)
	static bool HasStartFrame(const UMovieSceneSection* Section);
	UFUNCTION(BlueprintCallable)
	static bool HasEndFrame(const UMovieSceneSection* Section);
	UFUNCTION(BlueprintCallable)
	static FFrameNumber GetInclusiveStartFrame(const UMovieSceneSection* Section);
	UFUNCTION(BlueprintCallable)
	static FFrameNumber GetExclusiveEndFrame(const UMovieSceneSection* Section);
	//UFUNCTION(BlueprintCallable)
	//static void SetStartFrame(UMovieSceneSection* Section, FSequencerScriptingRange NewStartFrame, FFrameRate FrameRate);
	//UFUNCTION(BlueprintCallable)
	//static void SetEndFrame(UMovieSceneSection* Section, FSequencerScriptingRange NewEndFrame, FFrameRate FrameRate);
	UFUNCTION(BlueprintCallable)
	static bool IsTimeWithinSection(const UMovieSceneSection* Section, FFrameNumber Position);
	//UFUNCTION(BlueprintCallable)
	//static FSequencerScriptingRange GetAutoSizeRange(const UMovieSceneSection* Section, FFrameRate FrameRate);
	UFUNCTION(BlueprintPure)
	static EMovieSceneCompletionMode GetCompletionMode(const UMovieSceneSection* Section);
	UFUNCTION(BlueprintCallable)
	static void SetCompletionMode(UMovieSceneSection* Section, EMovieSceneCompletionMode InCompletionMode);
	UFUNCTION(BlueprintPure)
	static FOptionalMovieSceneBlendType GetBlendType(const UMovieSceneSection* Section);
	UFUNCTION(BlueprintCallable)
	static void SetBlendType(UMovieSceneSection* Section, EMovieSceneBlendType InBlendType);
	UFUNCTION(BlueprintCallable)
	static void GetSupportedBlendTypes(const UMovieSceneSection* Section, TArray<EMovieSceneBlendType>& SupportedBlendTypes);
	UFUNCTION(BlueprintCallable)
	static void MoveSection(UMovieSceneSection* Section, FFrameNumber DeltaTime);
	//UFUNCTION(BlueprintCallable)
	//static FSequencerScriptingRange ComputeEffectiveRange(UMovieSceneSection* Section);
	UFUNCTION(BlueprintCallable)
	static UMovieSceneSection* SplitSection(UMovieSceneSection* Section, FQualifiedFrameTime SplitTime, bool bDeleteKeys);
	UFUNCTION(BlueprintCallable)
	static void TrimSection(UMovieSceneSection* Section, FQualifiedFrameTime TrimTime, bool bTrimLeft, bool bDeleteKeys);
	UFUNCTION(BlueprintCallable)
	static void GetSnapTimes(const UMovieSceneSection* Section, TArray<FFrameNumber>& OutSnapTimes, bool bGetSectionBorders);
};
