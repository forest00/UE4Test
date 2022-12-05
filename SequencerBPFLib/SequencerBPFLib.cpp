


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
#if WITH_EDITORONLY_DATA
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
#endif

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

void UMovieSceneSectionBPFLib::GetAllKeySeconds(UMovieSceneSection* Section, FFrameRate FrameRate, TArray<float>& OutKeySeconds)
{
	//	FSequencerScriptingRange Range = FSequencerScriptingRange::FromNative(Section->GetRange(), FrameRate);
	//GetKeySeconds(Section, Range, FrameRate, OutKeySeconds);
}

void UMovieSceneSectionBPFLib::GetKeySeconds(UMovieSceneSection* Section, FSequencerScriptingRange WithinRange, FFrameRate FrameRate, TArray<float>& OutKeySeconds)
{
	if (Section == nullptr) { return; }

	FMovieSceneChannel* Channel = GetFirstChannel(Section);
	if (Channel != nullptr)
	{
		const float ValueFix = 1.0f / 800.0f / float(FrameRate.AsFrameNumber(1).Value);
		TRange<FFrameNumber> WithinRangeFrame = WithinRange.ToNative(FrameRate);
		TArray<FFrameNumber> OutKeyTimesFrame;
		TArray<FKeyHandle> OutKeyHandlesFrame;
		OutKeySeconds.Reset();
		if (Channel != nullptr)
		{
			Channel->GetKeys(WithinRangeFrame, &OutKeyTimesFrame, &OutKeyHandlesFrame);
			for (auto i = OutKeyTimesFrame.CreateConstIterator(); i; i++)
			{
				OutKeySeconds.Add(i->Value * ValueFix);
			}
		}
	}
}

bool UMovieSceneSectionBPFLib::IsReadOnly(const UMovieSceneSection* Section)
{
	if (Section == nullptr) { return false; }

	return Section->IsReadOnly();
}

FSequencerScriptingRange UMovieSceneSectionBPFLib::GetRange(const UMovieSceneSection* Section, FFrameRate FrameRate)
{
	if (Section == nullptr) { TRange<FFrameNumber> Range = TRange<FFrameNumber>(0, 0); return FSequencerScriptingRange::FromNative(Range, FrameRate); }

	return FSequencerScriptingRange::FromNative(Section->GetRange(), FrameRate);
}

FSequencerScriptingRange UMovieSceneSectionBPFLib::GetTrueRange(const UMovieSceneSection* Section, FFrameRate FrameRate)
{
	if (Section == nullptr) { TRange<FFrameNumber> Range = TRange<FFrameNumber>(0, 0); return FSequencerScriptingRange::FromNative(Range, FrameRate); }

	return FSequencerScriptingRange::FromNative(Section->GetTrueRange(), FrameRate);
}

bool UMovieSceneSectionBPFLib::HasStartFrame(const UMovieSceneSection* Section)
{
	if (Section == nullptr) { return false; }

	return Section->HasStartFrame();
}

bool UMovieSceneSectionBPFLib::HasEndFrame(const UMovieSceneSection* Section)
{
	if (Section == nullptr) { return false; }

	return Section->HasStartFrame();
}

FFrameNumber UMovieSceneSectionBPFLib::GetInclusiveStartFrame(const UMovieSceneSection* Section)
{
	if (Section == nullptr) { return 0; }

	return Section->GetInclusiveStartFrame();
}

FFrameNumber UMovieSceneSectionBPFLib::GetExclusiveEndFrame(const UMovieSceneSection* Section)
{
	if (Section == nullptr) { return 0; }

	return Section->GetExclusiveEndFrame();
}

//void UMovieSceneSectionBPFLib::SetStartFrame(UMovieSceneSection* Section, FSequencerScriptingRange NewStartFrame, FFrameRate FrameRate)
//{
//	if (Section == nullptr) { return; }
//
//	return Section->SetStartFrame(NewStartFrame.ToNative(FrameRate));
//}
//
//void UMovieSceneSectionBPFLib::SetEndFrame(UMovieSceneSection* Section, FSequencerScriptingRange NewEndFrame, FFrameRate FrameRate)
//{
//	if (Section == nullptr) { return; }
//
//	return Section->SetEndFrame(NewEndFrame.ToNative(FrameRate));
//}

bool UMovieSceneSectionBPFLib::IsTimeWithinSection(const UMovieSceneSection* Section, FFrameNumber Position)
{
	if (Section == nullptr) { return false; }

	return Section->IsTimeWithinSection(Position);
}

//FSequencerScriptingRange UMovieSceneSectionBPFLib::GetAutoSizeRange(const UMovieSceneSection* Section, FFrameRate FrameRate)
//{
//	if (Section == nullptr) { TRange<FFrameNumber> Range = TRange<FFrameNumber>(0, 0); return FSequencerScriptingRange::FromNative(Range, FrameRate); }
//
//	return FSequencerScriptingRange::FromNative(Section->GetAutoSizeRange(), FrameRate);
//}

EMovieSceneCompletionMode UMovieSceneSectionBPFLib::GetCompletionMode(const UMovieSceneSection* Section)
{
	if (Section == nullptr) { return EMovieSceneCompletionMode::ProjectDefault; }

	return Section->GetCompletionMode();
}

void UMovieSceneSectionBPFLib::SetCompletionMode(UMovieSceneSection* Section, EMovieSceneCompletionMode InCompletionMode)
{
	if (Section == nullptr) { return; }

	return Section->SetCompletionMode(InCompletionMode);
}

FOptionalMovieSceneBlendType UMovieSceneSectionBPFLib::GetBlendType(const UMovieSceneSection* Section)
{
	if (Section == nullptr) { return FOptionalMovieSceneBlendType(EMovieSceneBlendType::Invalid); }

	return Section->GetBlendType();
}

void UMovieSceneSectionBPFLib::SetBlendType(UMovieSceneSection* Section, EMovieSceneBlendType InBlendType)
{
	if (Section == nullptr) { return; }

	return Section->SetBlendType(InBlendType);
}

void UMovieSceneSectionBPFLib::GetSupportedBlendTypes(const UMovieSceneSection* Section, TArray<EMovieSceneBlendType>& SupportedBlendTypes)
{
	if (Section == nullptr) { return; }

	FMovieSceneBlendTypeField Field = Section->GetSupportedBlendTypes();
	TArray<EMovieSceneBlendType> Tests = {
		EMovieSceneBlendType::Invalid,
		EMovieSceneBlendType::Absolute,
		EMovieSceneBlendType::Additive,
		EMovieSceneBlendType::Relative,
		EMovieSceneBlendType::AdditiveFromBase,
	};
	for (const EMovieSceneBlendType& Test : Tests)
	{
		if (Field.Contains(Test))
		{
			SupportedBlendTypes.Add(Test);
		}
	}
}

void UMovieSceneSectionBPFLib::MoveSection(UMovieSceneSection* Section, FFrameNumber DeltaTime)
{
	if (Section == nullptr) { return; }

	return Section->MoveSection(DeltaTime);
}

//FSequencerScriptingRange UMovieSceneSectionBPFLib::ComputeEffectiveRange(UMovieSceneSection* Section)
//{
//	if (Section == nullptr) { TRange<FFrameNumber> Range = TRange<FFrameNumber>(0, 0); return FSequencerScriptingRange::FromNative(Range, FrameRate); }
//
//	return Section->ComputeEffectiveRange();
//}

UMovieSceneSection* UMovieSceneSectionBPFLib::SplitSection(UMovieSceneSection* Section, FQualifiedFrameTime SplitTime, bool bDeleteKeys)
{
	if (Section == nullptr) { return nullptr; }

	return Section->SplitSection(SplitTime, bDeleteKeys);
}

void UMovieSceneSectionBPFLib::TrimSection(UMovieSceneSection* Section, FQualifiedFrameTime TrimTime, bool bTrimLeft, bool bDeleteKeys)
{
	if (Section == nullptr) { return; }

	return Section->TrimSection(TrimTime, bTrimLeft, bDeleteKeys);
}

void UMovieSceneSectionBPFLib::GetSnapTimes(const UMovieSceneSection* Section, TArray<FFrameNumber>& OutSnapTimes, bool bGetSectionBorders)
{
	if (Section == nullptr) { return; }

	return Section->GetSnapTimes(OutSnapTimes, bGetSectionBorders);
}
