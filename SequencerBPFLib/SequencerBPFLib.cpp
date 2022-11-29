


#include "SequencerBPFLib.h"
#include "MovieScene.h"
#include "MovieSceneSequence.h"
#include "MovieSceneTrack.h"
#include "MovieSceneSection.h"
#include "MovieSceneSequence.h"
#include "MovieSceneNameableTrack.h"
#include "MovieSceneFolder.h"
#include "Channels/MovieSceneChannel.h"
#include "Channels/MovieSceneChannelProxy.h"
#include "Channels/MovieSceneEventChannel.h"
#include "Misc/FrameNumber.h"
#include "Curves/KeyHandle.h"

UMovieSceneTrack* USequencerBPFLib::FindTrackWithGUID(UMovieSceneSequence* Sequencer, TSubclassOf<UMovieSceneTrack> TrackClass, FGuid ObjectGuid, FName TrackName)
{
	if (Sequencer == nullptr || Sequencer->GetMovieScene() == nullptr) { return nullptr; }
	if (!ObjectGuid.IsValid()) { UE_LOG(LogTemp, Log, TEXT("!ObjectGuid.IsValid(): %s"), *ObjectGuid.ToString()); return nullptr; };

	return Sequencer->GetMovieScene()->FindTrack(TrackClass, ObjectGuid, TrackName);
}

UMovieSceneTrack* USequencerBPFLib::FindTrack(UMovieSceneSequence* Sequencer, TSubclassOf<UMovieSceneTrack> TrackClass, FText TrackName)
{
	if (Sequencer == nullptr || Sequencer->GetMovieScene() == nullptr) { return nullptr; }

	TArray<UMovieSceneTrack*> Tracks = Sequencer->GetMovieScene()->GetMasterTracks();
	for (UMovieSceneTrack* Track : Tracks)
	{
		if (TrackClass.GetDefaultObject() == nullptr || Track->GetClass() == TrackClass)
		{
			if (TrackName.IsEmpty() || Track->GetDisplayName().EqualTo(TrackName))
			{
				return Track;
			}
		}
	}

	return nullptr;
}

FGuid USequencerBPFLib::FindGUID(UMovieSceneSequence* Sequencer, TSubclassOf<UMovieSceneTrack> TrackClass, FText TrackName)
{
	UMovieSceneTrack* Track = FindTrack(Sequencer, TrackClass, TrackName);
	if (Track == nullptr) { return FGuid(); }

	return Track->FindObjectBindingGuid();
}

TArray<UMovieSceneSection*> USequencerBPFLib::GetAllSections(UMovieSceneSequence* Sequencer)
{
	if (Sequencer == nullptr || Sequencer->GetMovieScene() == nullptr) { return TArray<UMovieSceneSection*>{}; }

	return Sequencer->GetMovieScene()->GetAllSections();
}

void USequencerBPFLib::FindFolder(TArray<UMovieSceneFolder*> Folders, FName FolderName, bool& Found, UMovieSceneFolder*& FoundFolder)
{
	Found = false;
	FoundFolder = nullptr;
	for (UMovieSceneFolder* Folder : Folders)
	{
		if (Folder->GetFolderName() == FolderName)
		{
			Found = true;
			FoundFolder = Folder;
			return;
		}
	}
}

//----

FGuid UMovieSceneTrackBPFLib::FindObjectBindingGuid(const UMovieSceneTrack* Track)
{
	if (Track == nullptr) { return FGuid(); }

	return Track->FindObjectBindingGuid();
}

FText UMovieSceneTrackBPFLib::GetTrackName(const UMovieSceneNameableTrack* Track)
{
#if WITH_EDITORONLY_DATA
	if (Track == nullptr) { return FText::GetEmpty(); }

	return Track->GetDisplayName();
#endif
	return FText::GetEmpty();
}

TArray<UMovieSceneSection*> UMovieSceneTrackBPFLib::GetAllSections(const UMovieSceneTrack* Track)
{
	if (Track == nullptr) { return TArray<UMovieSceneSection*>{}; }

	return Track->GetAllSections();
}

int32 UMovieSceneTrackBPFLib::GetSectionsNum(const UMovieSceneTrack* Track)
{
	if (Track == nullptr) { return 0; }

	return Track->GetAllSections().Num();
}

UMovieSceneSection* UMovieSceneTrackBPFLib::GetSection(const UMovieSceneTrack* Track, int32 SectionIndex)
{
	if (Track == nullptr) { return nullptr; }

	const TArray<UMovieSceneSection*>& AllSections = Track->GetAllSections();
	if (AllSections.IsValidIndex(SectionIndex))
	{
		return AllSections[SectionIndex];
	}
	return nullptr;
}

//----

struct FMovieSceneChannel* UMovieSceneSectionBPFLib::GetChannel(UMovieSceneSection* Section, int32 EntryIndex, int32 ChannelIndex)
{
	if (Section == nullptr) { return 0; }

	FMovieSceneChannelProxy& ChannelProxy = Section->GetChannelProxy();
	TArrayView<const FMovieSceneChannelEntry> AllEntries = ChannelProxy.GetAllEntries();
	if (AllEntries.IsValidIndex(EntryIndex))
	{
		const FName ChannelTypeName = AllEntries[EntryIndex].GetChannelTypeName();
		FMovieSceneChannel* Channel = ChannelProxy.GetChannel(ChannelTypeName, ChannelIndex);
		return Channel;
	}
	return nullptr;
}

struct FMovieSceneChannel* UMovieSceneSectionBPFLib::GetFirstChannel(UMovieSceneSection* Section)
{
	return GetChannel(Section, 0, 0);
}

int32 UMovieSceneSectionBPFLib::GetChannelsNum(UMovieSceneSection* Section)
{
	if (Section == nullptr) { return 0; }

	FMovieSceneChannelProxy& ChannelProxy = Section->GetChannelProxy();
	TArrayView<const FMovieSceneChannelEntry> AllEntries = ChannelProxy.GetAllEntries();
	if (AllEntries.IsValidIndex(0))
	{
		return AllEntries[0].GetChannels().Num();
	}
	return 0;
}

FName UMovieSceneSectionBPFLib::GetFirstChannelTypeName(UMovieSceneSection* Section)
{
	if (Section == nullptr) { return NAME_None; }

	FMovieSceneChannelProxy& ChannelProxy = Section->GetChannelProxy();
	TArrayView<const FMovieSceneChannelEntry> AllEntries = ChannelProxy.GetAllEntries();
	if (AllEntries.IsValidIndex(0))
	{
		return AllEntries[0].GetChannelTypeName();
	}
	return NAME_None;
}

void UMovieSceneSectionBPFLib::GetAllChannelTypeNames(UMovieSceneSection* Section, TArray<FName>& EntryChannelTypeNames)
{
	if (Section == nullptr) { return; }

	FMovieSceneChannelProxy& ChannelProxy = Section->GetChannelProxy();
	TArrayView<const FMovieSceneChannelEntry> AllEntries = ChannelProxy.GetAllEntries();
	for (int32 i = 0; i < AllEntries.Num(); i++)
	{
		EntryChannelTypeNames.Add(AllEntries[i].GetChannelTypeName());
	}
}

void UMovieSceneSectionBPFLib::GetKeyTimes(UMovieSceneSection* Section, FInt32Range WithinRange, TArray<int32>& OutKeyTimes)
{
	if (Section == nullptr) { return; }

	FMovieSceneChannel* Channel = GetFirstChannel(Section);
	if (Channel != nullptr)
	{
		TRange<FFrameNumber> WithinRangeFrame = TRange<FFrameNumber>(FFrameNumber(WithinRange.GetLowerBoundValue()), FFrameNumber(WithinRange.GetUpperBoundValue()));
		TArray<FFrameNumber> OutKeyTimesFrame;
		TArray<FKeyHandle> OutKeyHandlesFrame;
		OutKeyTimes.Reset();
		if (Channel != nullptr)
		{
			Channel->GetKeys(WithinRangeFrame, &OutKeyTimesFrame, &OutKeyHandlesFrame);
			for (auto i = OutKeyTimesFrame.CreateConstIterator(); i; i++)
			{
				OutKeyTimes.Add(i->Value);
			}
		}
	}
}

void UMovieSceneSectionBPFLib::GetChannelKeyTimes(UMovieSceneSection* Section, int32 ChannelIndex, FInt32Range WithinRange, TArray<int32>& OutKeyTimes)
{
	if (Section == nullptr) { return; }

	FMovieSceneChannelProxy& ChannelProxy = Section->GetChannelProxy();
	FMovieSceneChannel* Channel = ChannelProxy.GetChannel<FMovieSceneEventChannel>(ChannelIndex);
	TRange<FFrameNumber> WithinRangeF = TRange<FFrameNumber>(FFrameNumber(WithinRange.GetLowerBoundValue()), FFrameNumber(WithinRange.GetUpperBoundValue()));
	TArray<FFrameNumber> OutKeyTimesF;
	TArray<FKeyHandle> OutKeyHandlesF;
	OutKeyTimes.Reset();
	if (Channel != nullptr)
	{
		Channel->GetKeys(WithinRangeF, &OutKeyTimesF, &OutKeyHandlesF);
		for (auto i = OutKeyTimesF.CreateConstIterator(); i; i++)
		{
			OutKeyTimes.Add(i->Value);
		}
	}
}
