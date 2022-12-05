

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
#include "Math\UnrealMathUtility.h"
#include "SequenceTimeUnit.h"
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
	static void GetKeyTimes(UMovieSceneSection* Section, FInt32Range WithinRange, TArray<int32>& KeyTimes);

	UFUNCTION(BlueprintCallable)
	static void GetChannelKeyTimes(UMovieSceneSection* Section, int32 ChannelIndex, FInt32Range WithinRange, TArray<int32>& KeyTimes);

	UFUNCTION(BlueprintCallable)
	void GetAllKeySeconds(UMovieSceneSection* Section, FFrameRate DisplayRate, FFrameRate TickResolution, TArray<float>& KeySeconds);

	UFUNCTION(BlueprintCallable)
	static void GetKeySeconds(UMovieSceneSection* Section, FSequencerScriptingRange WithinRange, FFrameRate DisplayRate, FFrameRate TickResolution, TArray<float>& KeySeconds);

	UFUNCTION(BlueprintCallable)
	static bool IsReadOnly(const UMovieSceneSection* Section);

	UFUNCTION(BlueprintCallable)
	static FSequencerScriptingRange GetRange(const UMovieSceneSection* Section, FFrameRate DisplayRate);

	UFUNCTION(BlueprintCallable)
	static FSequencerScriptingRange GetTrueRange(const UMovieSceneSection* Section, FFrameRate DisplayRate);

	UFUNCTION(BlueprintCallable)
	static bool HasStartFrame(const UMovieSceneSection* Section);
	UFUNCTION(BlueprintCallable)
	static bool HasEndFrame(const UMovieSceneSection* Section);
	UFUNCTION(BlueprintCallable)
	static FFrameNumber GetInclusiveStartFrame(const UMovieSceneSection* Section);
	UFUNCTION(BlueprintCallable)
	static FFrameNumber GetExclusiveEndFrame(const UMovieSceneSection* Section);
	//UFUNCTION(BlueprintCallable)
	//static void SetStartFrame(UMovieSceneSection* Section, FSequencerScriptingRange NewStartFrame, FFrameRate DisplayRate);
	//UFUNCTION(BlueprintCallable)
	//static void SetEndFrame(UMovieSceneSection* Section, FSequencerScriptingRange NewEndFrame, FFrameRate DisplayRate);
	UFUNCTION(BlueprintCallable)
	static bool IsTimeWithinSection(const UMovieSceneSection* Section, FFrameNumber Position);
	//UFUNCTION(BlueprintCallable)
	//static FSequencerScriptingRange GetAutoSizeRange(const UMovieSceneSection* Section, FFrameRate DisplayRate);
	UFUNCTION(BlueprintPure)
	static EMovieSceneCompletionMode GetCompletionMode(const UMovieSceneSection* Section);
	UFUNCTION(BlueprintCallable)
	static void SetCompletionMode(UMovieSceneSection* Section, EMovieSceneCompletionMode CompletionMode);
	UFUNCTION(BlueprintPure)
	static FOptionalMovieSceneBlendType GetBlendType(const UMovieSceneSection* Section);
	UFUNCTION(BlueprintCallable)
	static void SetBlendType(UMovieSceneSection* Section, EMovieSceneBlendType BlendType);
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
	static void GetSnapTimes(const UMovieSceneSection* Section, TArray<FFrameNumber>& SnapTimes, bool bGetSectionBorders);
};

USTRUCT(BlueprintType)
struct FMovieSceneKeyHandle
{

	GENERATED_BODY();

protected:

	FKeyHandle Handle;

public:

	FMovieSceneKeyHandle();

	FMovieSceneKeyHandle(FKeyHandle Handle);

	FMovieSceneKeyHandle(FKeyHandle* HandlePtr);

	FKeyHandle& operator*();

	const FKeyHandle& operator*() const;

};

UCLASS(MinimalAPI, BlueprintType)
class UMovieSceneChannelBPFLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	static struct FMovieSceneChannel* UMovieSceneChannelBPFLib::GetChannel(UMovieSceneSection* Section, int32 EntryIndex, int32 ChannelIndex);

	static const struct FMovieSceneChannel* GetChannel(const UMovieSceneSection* Section, int32 EntryIndex, int32 ChannelIndex);

	static struct FMovieSceneChannel* GetFirstChannel(UMovieSceneSection* Section);

	static const struct FMovieSceneChannel* GetFirstChannel(const UMovieSceneSection* Section);

	static void ToKeyHandles(TArray<FMovieSceneKeyHandle> KeyHandles, TArray<FKeyHandle>& OutKeyHandles);

	static void ToKeyHandles(TArray<FKeyHandle> KeyHandles, TArray<FMovieSceneKeyHandle>& OutKeyHandles);

	// ---- Times ----

	UFUNCTION(BlueprintCallable)
	static void FramesToTimes(TArray<FFrameNumber> Frames, TArray<FFrameTime>& Times);

	UFUNCTION(BlueprintCallable)
	static void TimesToFrames(TArray<FFrameTime> Times, TArray<FFrameNumber>& Frames);

	UFUNCTION(BlueprintCallable)
	static void TransformFrame(FFrameNumber SourceFrame, FFrameRate SourceRate, FFrameRate DestinationRate, FFrameTime& DestinationTime);

	UFUNCTION(BlueprintCallable)
	static void TransformFrames(TArray<FFrameNumber> SourceFrames, FFrameRate SourceRate, FFrameRate DestinationRate, TArray<FFrameTime>& DestinationTimes);

	UFUNCTION(BlueprintCallable)
	static void TransformTime(FFrameTime SourceTime, FFrameRate SourceRate, FFrameRate DestinationRate, FFrameTime& DestinationTime);

	UFUNCTION(BlueprintCallable)
	static void TransformTimes(TArray<FFrameTime> SourceTimes, FFrameRate SourceRate, FFrameRate DestinationRate, TArray<FFrameTime>& DestinationTimes);

	UFUNCTION(BlueprintCallable)
	static void SnapTime(FFrameTime SourceTime, FFrameRate SourceRate, FFrameRate SnapToRate, FFrameTime& DestinationTime);

	UFUNCTION(BlueprintCallable)
	static void SnapTimes(TArray<FFrameTime> SourceTimes, FFrameRate SourceRate, FFrameRate SnapToRate, TArray<FFrameTime>& DestinationTimes);

	UFUNCTION(BlueprintCallable)
	static void ToSecond(FFrameNumber FrameNumber, FFrameRate DisplayRate, FFrameRate TickResolution, ESequenceTimeUnit SequenceTimeUnit, float& Second);

	UFUNCTION(BlueprintCallable)
	static void ToSeconds(TArray<FFrameNumber> FrameNumbers, FFrameRate DisplayRate, FFrameRate TickResolution, ESequenceTimeUnit SequenceTimeUnit, TArray<float>& Second);

	UFUNCTION(BlueprintCallable)
	static void ToFrameNumber(float Second, FFrameRate DisplayRate, FFrameRate TickResolution, ESequenceTimeUnit SequenceTimeUnit, FFrameNumber& FrameNumber);

	UFUNCTION(BlueprintCallable)
	static void ToFrameNumbers(TArray<float> Seconds, FFrameRate DisplayRate, FFrameRate TickResolution, ESequenceTimeUnit SequenceTimeUnit, TArray<FFrameNumber>& FrameNumbers);

	// ---- Keys ----

	UFUNCTION(BlueprintCallable)
	static int32 GetNumKeys(const UMovieSceneSection* Section);

	UFUNCTION(BlueprintCallable)
	static void GetKeys(UMovieSceneSection* Section, FFrameNumberRange WithinRange, TArray<FMovieSceneKeyHandle>& KeyHandles);

	UFUNCTION(BlueprintCallable)
	static void GetAllKeys(UMovieSceneSection* Section, TArray<FMovieSceneKeyHandle>& KeyHandles);

	UFUNCTION(BlueprintCallable)
	static int32 GetHash(FMovieSceneKeyHandle KeyHandle);

	// ---- Key Positions ----

	UFUNCTION(BlueprintCallable)
	static void GetKeyFrameNumberRaw(UMovieSceneSection* Section, FMovieSceneKeyHandle Handle, FFrameNumber& KeyFrameNumber);

	UFUNCTION(BlueprintCallable)
	static void GetKeyFrameNumbersRaw(UMovieSceneSection* Section, TArray<FMovieSceneKeyHandle> Handles, TArray<FFrameNumber>& KeyFrameNumbers);

	UFUNCTION(BlueprintCallable)
	static void GetKeyFrameNumber(UMovieSceneSequence* Sequencer, UMovieSceneSection* Section, FMovieSceneKeyHandle Handle, ESequenceTimeUnit SequenceTimeUnit, FFrameNumber& KeyFrameNumber);

	UFUNCTION(BlueprintCallable)
	static void GetKeyFrameNumbers(UMovieSceneSequence* Sequencer, UMovieSceneSection* Section, TArray<FMovieSceneKeyHandle> Handles, ESequenceTimeUnit SequenceTimeUnit, TArray<FFrameNumber>& KeyFrameNumbers);

	UFUNCTION(BlueprintCallable)
	static void GetKeySecond(UMovieSceneSequence* Sequencer, UMovieSceneSection* Section, FMovieSceneKeyHandle Handle, float& KeySecond);

	UFUNCTION(BlueprintCallable)
	static void GetKeySeconds(UMovieSceneSequence* Sequencer, UMovieSceneSection* Section, TArray<FMovieSceneKeyHandle> Handles, TArray<float>& KeySeconds);

	UFUNCTION(BlueprintCallable)
	static void SetKeyFrameNumberRaw(UMovieSceneSection* Section, FMovieSceneKeyHandle Handle, FFrameNumber KeyFrameNumber);

	UFUNCTION(BlueprintCallable)
	static void SetKeyFrameNumbersRaw(UMovieSceneSection* Section, TArray<FMovieSceneKeyHandle> Handles, TArray<FFrameNumber> KeyFrameNumbers);

	UFUNCTION(BlueprintCallable)
	static void SetKeyFrameNumber(UMovieSceneSequence* Sequencer, UMovieSceneSection* Section, FMovieSceneKeyHandle Handle, ESequenceTimeUnit SequenceTimeUnit, FFrameNumber KeyFrameNumber);

	UFUNCTION(BlueprintCallable)
	static void SetKeyFrameNumbers(UMovieSceneSequence* Sequencer, UMovieSceneSection* Section, TArray<FMovieSceneKeyHandle> Handles, ESequenceTimeUnit SequenceTimeUnit, TArray<FFrameNumber> KeyFrameNumbers);
	
	UFUNCTION(BlueprintCallable)
	static void SetKeySecond(UMovieSceneSequence* Sequencer, UMovieSceneSection* Section, FMovieSceneKeyHandle Handle, float KeySecond);

	UFUNCTION(BlueprintCallable)
	static void SetKeySeconds(UMovieSceneSequence* Sequencer, UMovieSceneSection* Section, TArray<FMovieSceneKeyHandle> Handles, TArray<float> KeySeconds);

	// ---- Key Edit ----

	UFUNCTION(BlueprintCallable)
	static void DuplicateKey(UMovieSceneSection* Section, FMovieSceneKeyHandle Handle, FMovieSceneKeyHandle& NewHandle);

	UFUNCTION(BlueprintCallable)
	static void DuplicateKeys(UMovieSceneSection* Section, TArray<FMovieSceneKeyHandle> Handles, TArray<FMovieSceneKeyHandle>& NewHandles);

	UFUNCTION(BlueprintCallable)
	static void DeleteKey(UMovieSceneSection* Section, FMovieSceneKeyHandle Handle);
	
	UFUNCTION(BlueprintCallable)
	static void DeleteKeys(UMovieSceneSection* Section, TArray<FMovieSceneKeyHandle> Handles);
	
	UFUNCTION(BlueprintCallable)
	static void DeleteKeysFrom(UMovieSceneSection* Section, FFrameNumber FrameNumber, bool bDeleteKeysBefore);
	
	// ---- Channel Edit ----

	UFUNCTION(BlueprintCallable)
	static void ChangeFrameResolution(UMovieSceneSection* Section, FFrameRate SourceRate, FFrameRate DestinationRate);
	
	UFUNCTION(BlueprintCallable)
	static FFrameNumberRange ComputeEffectiveRange(const UMovieSceneSection* Section);
	
	UFUNCTION(BlueprintCallable)
	static void Reset(UMovieSceneSection* Section);
	
	UFUNCTION(BlueprintCallable)
	static void Offset(UMovieSceneSection* Section, FFrameNumber DeltaPosition);
	
	UFUNCTION(BlueprintCallable)
	static void Optimize(UMovieSceneSection* Section, FFrameNumberRange Range, FFrameRate DisplayRate, float Tolerance = 1.e-4f, bool bAutoSetInterpolation = false);
	
	UFUNCTION(BlueprintCallable)
	static void ClearDefault(UMovieSceneSection* Section);
	
	UFUNCTION(BlueprintCallable)
	static void PostEditChange(UMovieSceneSection* Section);
};
