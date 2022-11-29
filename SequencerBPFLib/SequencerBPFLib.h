

#pragma once

#include "CoreMinimal.h"
#include "Math/Range.h"
#include "Kismet/BlueprintFunctionLibrary.h"
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

};
