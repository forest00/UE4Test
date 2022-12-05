


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
#include "Channels/MovieSceneChannelTraits.h"
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

UMovieSceneSection* UMovieSceneTrackBPFLib::GetSection(const UMovieSceneTrack* Track, int32 Sectiondex)
{
	if (Track == nullptr) { return nullptr; }

	const TArray<UMovieSceneSection*>& AllSections = Track->GetAllSections();
	if (AllSections.IsValidIndex(Sectiondex))
	{
		return AllSections[Sectiondex];
	}
	return nullptr;
}

//----

struct FMovieSceneChannel* UMovieSceneSectionBPFLib::GetChannel(UMovieSceneSection* Section, int32 Entrydex, int32 Channeldex)
{
	if (Section == nullptr) { return 0; }

	FMovieSceneChannelProxy& ChannelProxy = Section->GetChannelProxy();
	TArrayView<const FMovieSceneChannelEntry> AllEntries = ChannelProxy.GetAllEntries();
	if (AllEntries.IsValidIndex(Entrydex))
	{
		const FName ChannelTypeName = AllEntries[Entrydex].GetChannelTypeName();
		FMovieSceneChannel* Channel = ChannelProxy.GetChannel(ChannelTypeName, Channeldex);
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

void UMovieSceneSectionBPFLib::GetKeyTimes(UMovieSceneSection* Section, FInt32Range WithinRange, TArray<int32>& KeyTimes)
{
	if (Section == nullptr) { return; }

	FMovieSceneChannel* Channel = GetFirstChannel(Section);
	if (Channel != nullptr)
	{
		TRange<FFrameNumber> WithinRangeFrame = TRange<FFrameNumber>(FFrameNumber(WithinRange.GetLowerBoundValue()), FFrameNumber(WithinRange.GetUpperBoundValue()));
		TArray<FFrameNumber> KeyTimesFrame;
		TArray<FKeyHandle> KeyHandlesFrame;
		KeyTimes.Reset();
		if (Channel != nullptr)
		{
			Channel->GetKeys(WithinRangeFrame, &KeyTimesFrame, &KeyHandlesFrame);
			for (auto i = KeyTimesFrame.CreateConstIterator(); i; i++)
			{
				KeyTimes.Add(i->Value);
			}
		}
	}
}

void UMovieSceneSectionBPFLib::GetChannelKeyTimes(UMovieSceneSection* Section, int32 Channeldex, FInt32Range WithinRange, TArray<int32>& KeyTimes)
{
	if (Section == nullptr) { return; }

	FMovieSceneChannelProxy& ChannelProxy = Section->GetChannelProxy();
	FMovieSceneChannel* Channel = ChannelProxy.GetChannel<FMovieSceneEventChannel>(Channeldex);
	TRange<FFrameNumber> WithinRangeFrame = TRange<FFrameNumber>(FFrameNumber(WithinRange.GetLowerBoundValue()), FFrameNumber(WithinRange.GetUpperBoundValue()));
	TArray<FFrameNumber> KeyTimesFrame;
	TArray<FKeyHandle> KeyHandlesFrame;
	KeyTimes.Reset();
	if (Channel != nullptr)
	{
		Channel->GetKeys(WithinRangeFrame, &KeyTimesFrame, &KeyHandlesFrame);
		for (auto i = KeyTimesFrame.CreateConstIterator(); i; i++)
		{
			KeyTimes.Add(i->Value);
		}
	}
}

void UMovieSceneSectionBPFLib::GetAllKeySeconds(UMovieSceneSection* Section, FFrameRate DisplayRate, FFrameRate TickResolution, TArray<float>& KeySeconds)
{
	FSequencerScriptingRange Range = FSequencerScriptingRange::FromNative(TRange<FFrameNumber>::All(), DisplayRate);
	GetKeySeconds(Section, Range, DisplayRate, TickResolution, KeySeconds);
}

void UMovieSceneSectionBPFLib::GetKeySeconds(UMovieSceneSection* Section, FSequencerScriptingRange WithinRange, FFrameRate DisplayRate, FFrameRate TickResolution, TArray<float>& KeySeconds)
{
	if (Section == nullptr) { return; }

	FMovieSceneChannel* Channel = GetFirstChannel(Section);
	if (Channel != nullptr)
	{
		const float ValueFix = 1.0f / 30.0f;
		TRange<FFrameNumber> WithinRangeFrame = WithinRange.ToNative(DisplayRate);
		TArray<FFrameNumber> KeyTimesFrame;
		TArray<FKeyHandle> KeyHandlesFrame;
		KeySeconds.Reset();
		if (Channel != nullptr)
		{
			Channel->GetKeys(WithinRangeFrame, &KeyTimesFrame, &KeyHandlesFrame);
			for (auto i = KeyTimesFrame.CreateConstIterator(); i; i++)
			{
				FFrameTime Frame = FFrameRate::TransformTime(i->Value, TickResolution, DisplayRate);
				KeySeconds.Add(float(Frame.AsDecimal()) * ValueFix);
			}
		}
	}
}

bool UMovieSceneSectionBPFLib::IsReadOnly(const UMovieSceneSection* Section)
{
	if (Section == nullptr) { return false; }

	return Section->IsReadOnly();
}

FSequencerScriptingRange UMovieSceneSectionBPFLib::GetRange(const UMovieSceneSection* Section, FFrameRate DisplayRate)
{
	if (Section == nullptr) { TRange<FFrameNumber> Range = TRange<FFrameNumber>(0, 0); return FSequencerScriptingRange::FromNative(Range, DisplayRate); }

	return FSequencerScriptingRange::FromNative(Section->GetRange(), DisplayRate);
}

FSequencerScriptingRange UMovieSceneSectionBPFLib::GetTrueRange(const UMovieSceneSection* Section, FFrameRate DisplayRate)
{
	if (Section == nullptr) { TRange<FFrameNumber> Range = TRange<FFrameNumber>(0, 0); return FSequencerScriptingRange::FromNative(Range, DisplayRate); }

	return FSequencerScriptingRange::FromNative(Section->GetTrueRange(), DisplayRate);
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

//void UMovieSceneSectionBPFLib::SetStartFrame(UMovieSceneSection* Section, FSequencerScriptingRange NewStartFrame, FFrameRate DisplayRate)
//{
//	if (Section == nullptr) { return; }
//
//	return Section->SetStartFrame(NewStartFrame.ToNative(FrameRate));
//}
//
//void UMovieSceneSectionBPFLib::SetEndFrame(UMovieSceneSection* Section, FSequencerScriptingRange NewEndFrame, FFrameRate DisplayRate)
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

//FSequencerScriptingRange UMovieSceneSectionBPFLib::GetAutoSizeRange(const UMovieSceneSection* Section, FFrameRate DisplayRate)
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

void UMovieSceneSectionBPFLib::SetCompletionMode(UMovieSceneSection* Section, EMovieSceneCompletionMode CompletionMode)
{
	if (Section == nullptr) { return; }

	return Section->SetCompletionMode(CompletionMode);
}

FOptionalMovieSceneBlendType UMovieSceneSectionBPFLib::GetBlendType(const UMovieSceneSection* Section)
{
	if (Section == nullptr) { return FOptionalMovieSceneBlendType(EMovieSceneBlendType::Invalid); }

	return Section->GetBlendType();
}

void UMovieSceneSectionBPFLib::SetBlendType(UMovieSceneSection* Section, EMovieSceneBlendType BlendType)
{
	if (Section == nullptr) { return; }

	return Section->SetBlendType(BlendType);
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

void UMovieSceneSectionBPFLib::GetSnapTimes(const UMovieSceneSection* Section, TArray<FFrameNumber>& SnapTimes, bool bGetSectionBorders)
{
	if (Section == nullptr) { return; }

	return Section->GetSnapTimes(SnapTimes, bGetSectionBorders);
}

//----

FMovieSceneKeyHandle::FMovieSceneKeyHandle()
	: Handle(FKeyHandle::Invalid())
{

}

FMovieSceneKeyHandle::FMovieSceneKeyHandle(FKeyHandle Handle)
	: Handle(Handle)
{

}

FMovieSceneKeyHandle::FMovieSceneKeyHandle(FKeyHandle* HandlePtr)
	: Handle(*HandlePtr)
{

}

FKeyHandle& FMovieSceneKeyHandle::operator*()
{
	return Handle;
}

const FKeyHandle& FMovieSceneKeyHandle::operator*() const
{
	return Handle;
}

struct FMovieSceneChannel* UMovieSceneChannelBPFLib::GetChannel(UMovieSceneSection* Section, int32 Entrydex, int32 Channeldex)
{
	if (Section == nullptr) { return 0; }

	FMovieSceneChannelProxy& ChannelProxy = Section->GetChannelProxy();
	TArrayView<const FMovieSceneChannelEntry> AllEntries = ChannelProxy.GetAllEntries();
	if (AllEntries.IsValidIndex(Entrydex))
	{
		const FName ChannelTypeName = AllEntries[Entrydex].GetChannelTypeName();
		FMovieSceneChannel* Channel = ChannelProxy.GetChannel(ChannelTypeName, Channeldex);
		return Channel;
	}
	return nullptr;
}

const struct FMovieSceneChannel* UMovieSceneChannelBPFLib::GetChannel(const UMovieSceneSection* Section, int32 Entrydex, int32 Channeldex)
{
	if (Section == nullptr) { return 0; }

	FMovieSceneChannelProxy& ChannelProxy = Section->GetChannelProxy();
	TArrayView<const FMovieSceneChannelEntry> AllEntries = ChannelProxy.GetAllEntries();
	if (AllEntries.IsValidIndex(Entrydex))
	{
		const FName ChannelTypeName = AllEntries[Entrydex].GetChannelTypeName();
		FMovieSceneChannel* Channel = ChannelProxy.GetChannel(ChannelTypeName, Channeldex);
		return Channel;
	}
	return nullptr;
}

struct FMovieSceneChannel* UMovieSceneChannelBPFLib::GetFirstChannel(UMovieSceneSection* Section)
{
	return GetChannel(Section, 0, 0);
}

const struct FMovieSceneChannel* UMovieSceneChannelBPFLib::GetFirstChannel(const UMovieSceneSection* Section)
{
	return GetChannel(Section, 0, 0);
}

void UMovieSceneChannelBPFLib::ToKeyHandles(TArray<FMovieSceneKeyHandle> KeyHandles, TArray<FKeyHandle>& OutKeyHandles)
{
	OutKeyHandles.Reset();
	for (FMovieSceneKeyHandle& KeyHandle : KeyHandles)
	{
		OutKeyHandles.Add(*KeyHandle);
	}
}

void UMovieSceneChannelBPFLib::ToKeyHandles(TArray<FKeyHandle> KeyHandles, TArray<FMovieSceneKeyHandle>& OutKeyHandles)
{
	OutKeyHandles.Reset();
	for (FKeyHandle& KeyHandle : KeyHandles)
	{
		OutKeyHandles.Add(&KeyHandle);
	}
}

void UMovieSceneChannelBPFLib::FramesToTimes(TArray<FFrameNumber> Frames, TArray<FFrameTime>& Times)
{
	Times.Reset();
	for (FFrameNumber Frame : Frames)
	{
		Times.Add(Frame);
	}
}

void UMovieSceneChannelBPFLib::TimesToFrames(TArray<FFrameTime> Times, TArray<FFrameNumber>& Frames)
{
	Frames.Reset();
	for (FFrameTime Time : Times)
	{
		Frames.Add(Time.FloorToFrame());
	}
}

void UMovieSceneChannelBPFLib::TransformFrame(FFrameNumber SourceFrame, FFrameRate SourceRate, FFrameRate DestinationRate, FFrameTime& DestinationTime)
{
	DestinationTime = FFrameRate::TransformTime(SourceFrame, SourceRate, DestinationRate);
}

void UMovieSceneChannelBPFLib::TransformFrames(TArray<FFrameNumber> SourceFrames, FFrameRate SourceRate, FFrameRate DestinationRate, TArray<FFrameTime>& DestinationTimes)
{
	for (FFrameNumber SourceFrame : SourceFrames)
	{
		DestinationTimes.Add(FFrameRate::TransformTime(SourceFrame, SourceRate, DestinationRate));
	}
}

void UMovieSceneChannelBPFLib::TransformTime(FFrameTime SourceTime, FFrameRate SourceRate, FFrameRate DestinationRate, FFrameTime& DestinationTime)
{
	DestinationTime = FFrameRate::TransformTime(SourceTime, SourceRate, DestinationRate);
}

void UMovieSceneChannelBPFLib::TransformTimes(TArray<FFrameTime> SourceTimes, FFrameRate SourceRate, FFrameRate DestinationRate, TArray<FFrameTime>& DestinationTimes)
{
	for (FFrameTime& SourceTime : SourceTimes)
	{
		DestinationTimes.Add(FFrameRate::TransformTime(SourceTime, SourceRate, DestinationRate));
	}
}

void UMovieSceneChannelBPFLib::SnapTime(FFrameTime SourceTime, FFrameRate SourceRate, FFrameRate SnapToRate, FFrameTime& DestinationTime)
{
	DestinationTime = FFrameRate::Snap(SourceTime, SourceRate, SnapToRate);
}

void UMovieSceneChannelBPFLib::SnapTimes(TArray<FFrameTime> SourceTimes, FFrameRate SourceRate, FFrameRate SnapToRate, TArray<FFrameTime>& DestinationTimes)
{
	for (FFrameTime& SourceTime : SourceTimes)
	{
		DestinationTimes.Add(FFrameRate::Snap(SourceTime, SourceRate, SnapToRate));
	}
}

void UMovieSceneChannelBPFLib::ToSecond(FFrameNumber FrameNumber, FFrameRate DisplayRate, FFrameRate TickResolution, ESequenceTimeUnit SequenceTimeUnit, float& Second)
{
	FFrameTime Frame = FFrameTime();
	switch (SequenceTimeUnit)
	{
	case ESequenceTimeUnit::DisplayRate:
	{
		Frame = FFrameTime(FrameNumber);
	}
	case ESequenceTimeUnit::TickResolution:
	{
		Frame = FFrameRate::TransformTime(FFrameTime(FrameNumber), TickResolution, DisplayRate);
	}
	}
	Second = float(DisplayRate.AsSeconds(Frame));
}

void UMovieSceneChannelBPFLib::ToSeconds(TArray<FFrameNumber> FrameNumbers, FFrameRate DisplayRate, FFrameRate TickResolution, ESequenceTimeUnit SequenceTimeUnit, TArray<float>& Second)
{
	FFrameTime Frame = FFrameTime();
	switch (SequenceTimeUnit)
	{
	case ESequenceTimeUnit::DisplayRate:
		for (FFrameNumber FrameNumber : FrameNumbers)
		{
			Frame = FFrameTime(FrameNumber);
			Second.Add(float(DisplayRate.AsSeconds(Frame)));
		}
	case ESequenceTimeUnit::TickResolution:
		for (FFrameNumber FrameNumber : FrameNumbers)
		{
			Frame = FFrameRate::TransformTime(FFrameTime(FrameNumber), TickResolution, DisplayRate);
			Second.Add(float(DisplayRate.AsSeconds(Frame)));
		}
	}
}

void UMovieSceneChannelBPFLib::ToFrameNumber(float Second, FFrameRate DisplayRate, FFrameRate TickResolution, ESequenceTimeUnit SequenceTimeUnit, FFrameNumber& FrameNumber)
{
	switch (SequenceTimeUnit)
	{
	case ESequenceTimeUnit::DisplayRate:
	{
		FrameNumber = DisplayRate.AsFrameNumber(Second);
	}
	case ESequenceTimeUnit::TickResolution:
	{
		FrameNumber = TickResolution.AsFrameNumber(Second);
	}
	}
}

void UMovieSceneChannelBPFLib::ToFrameNumbers(TArray<float> Seconds, FFrameRate DisplayRate, FFrameRate TickResolution, ESequenceTimeUnit SequenceTimeUnit, TArray<FFrameNumber>& FrameNumbers)
{
	switch (SequenceTimeUnit)
	{
	case ESequenceTimeUnit::DisplayRate:
		for (const float& Second : Seconds)
		{
			FrameNumbers.Add(DisplayRate.AsFrameNumber(Second));
		}
	case ESequenceTimeUnit::TickResolution:
		for (const float& Second : Seconds)
		{
			FrameNumbers.Add(TickResolution.AsFrameNumber(Second));
		}
	}
}

int32 UMovieSceneChannelBPFLib::GetNumKeys(const UMovieSceneSection* Section)
{
	if (Section == nullptr) { return 0; }

	const FMovieSceneChannel* Channel = GetFirstChannel(Section);
	if (Channel != nullptr)
	{
		Channel->GetNumKeys();
	}

	return 0;
}

void UMovieSceneChannelBPFLib::GetKeys(UMovieSceneSection* Section, FFrameNumberRange WithinRange, TArray<FMovieSceneKeyHandle>& KeyHandles)
{
	if (Section == nullptr) { return; }

	FMovieSceneChannel* Channel = GetFirstChannel(Section);
	if (Channel != nullptr)
	{
		TArray<FFrameNumber> KeyFrameNumbers;
		TArray<FKeyHandle> Keys;
		Channel->GetKeys(WithinRange, &KeyFrameNumbers, &Keys);
		ToKeyHandles(Keys, KeyHandles);
	}
}

void UMovieSceneChannelBPFLib::GetAllKeys(UMovieSceneSection* Section, TArray<FMovieSceneKeyHandle>& KeyHandles)
{
	if (Section == nullptr) { return; }

	FMovieSceneChannel* Channel = GetFirstChannel(Section);
	if (Channel != nullptr)
	{
		FFrameNumberRange WithinRange = FFrameNumberRange::All();
		TArray<FFrameNumber> KeyFrameNumbers;
		TArray<FKeyHandle> Keys;
		Channel->GetKeys(WithinRange, &KeyFrameNumbers, &Keys);
		ToKeyHandles(Keys, KeyHandles);
	}
}

int32 UMovieSceneChannelBPFLib::GetHash(FMovieSceneKeyHandle KeyHandle)
{
	return int32(GetTypeHash(*KeyHandle));
}

void UMovieSceneChannelBPFLib::GetKeyFrameNumberRaw(UMovieSceneSection* Section, FMovieSceneKeyHandle Handle, FFrameNumber& KeyFrameNumber)
{
	if (Section == nullptr) { return; }

	FMovieSceneChannel* Channel = GetFirstChannel(Section);
	if (Channel != nullptr)
	{
		Channel->GetKeyTime(*Handle, KeyFrameNumber);
	}
}

void UMovieSceneChannelBPFLib::GetKeyFrameNumbersRaw(UMovieSceneSection* Section, TArray<FMovieSceneKeyHandle> Handles, TArray<FFrameNumber>& KeyFrameNumbers)
{
	if (Section == nullptr) { return; }

	FMovieSceneChannel* Channel = GetFirstChannel(Section);
	if (Channel != nullptr)
	{
		TArray<FKeyHandle> KeyHandles;
		ToKeyHandles(Handles, KeyHandles);
		KeyFrameNumbers.SetNum(KeyHandles.Num());
		Channel->GetKeyTimes(KeyHandles, KeyFrameNumbers);
	}
}

void UMovieSceneChannelBPFLib::GetKeyFrameNumber(UMovieSceneSequence* Sequencer, UMovieSceneSection* Section, FMovieSceneKeyHandle Handle, ESequenceTimeUnit SequenceTimeUnit, FFrameNumber& KeyFrameNumber)
{
	if ((Sequencer == nullptr) || Sequencer->GetMovieScene() == nullptr) { return; }
	if (Section == nullptr) { return; }

	GetKeyFrameNumberRaw(Section, Handle, KeyFrameNumber);
	switch (SequenceTimeUnit)
	{
	case ESequenceTimeUnit::DisplayRate:
		{
			FFrameRate DisplayRate = Sequencer->GetMovieScene()->GetDisplayRate();
			FFrameRate TickResolution = Sequencer->GetMovieScene()->GetTickResolution();
			FFrameTime Time;
			TransformFrame(KeyFrameNumber, TickResolution, DisplayRate, Time);
			KeyFrameNumber = Time.FloorToFrame();
		}
	}
}

void UMovieSceneChannelBPFLib::GetKeyFrameNumbers(UMovieSceneSequence* Sequencer, UMovieSceneSection* Section, TArray<FMovieSceneKeyHandle> Handles, ESequenceTimeUnit SequenceTimeUnit, TArray<FFrameNumber>& KeyFrameNumbers)
{
	if ((Sequencer == nullptr) || Sequencer->GetMovieScene() == nullptr) { return; }
	if (Section == nullptr) { return; }

	GetKeyFrameNumbersRaw(Section, Handles, KeyFrameNumbers);
	switch (SequenceTimeUnit)
	{
	case ESequenceTimeUnit::DisplayRate:
	{
		FFrameRate DisplayRate = Sequencer->GetMovieScene()->GetDisplayRate();
		FFrameRate TickResolution = Sequencer->GetMovieScene()->GetTickResolution();
		TArray<FFrameTime> Times;
		TransformFrames(KeyFrameNumbers, TickResolution, DisplayRate, Times);
		TimesToFrames(Times, KeyFrameNumbers);
	}
	}
}

void UMovieSceneChannelBPFLib::GetKeySecond(UMovieSceneSequence* Sequencer, UMovieSceneSection* Section, FMovieSceneKeyHandle Handle, float& KeySecond)
{
	if ((Sequencer == nullptr) || Sequencer->GetMovieScene() == nullptr) { return; }
	if (Section == nullptr) { return; }

	FFrameNumber KeyFrameNumber;
	GetKeyFrameNumberRaw(Section, Handle, KeyFrameNumber);
	FFrameRate DisplayRate = Sequencer->GetMovieScene()->GetDisplayRate();
	FFrameRate TickResolution = Sequencer->GetMovieScene()->GetTickResolution();
	ToSecond(KeyFrameNumber, DisplayRate, TickResolution, ESequenceTimeUnit::TickResolution, KeySecond);
}

void UMovieSceneChannelBPFLib::GetKeySeconds(UMovieSceneSequence* Sequencer, UMovieSceneSection* Section, TArray<FMovieSceneKeyHandle> Handles, TArray<float>& KeySeconds)
{
	if ((Sequencer == nullptr) || Sequencer->GetMovieScene() == nullptr) { return; }
	if (Section == nullptr) { return; }

	TArray<FFrameNumber> KeyFrameNumbers;
	GetKeyFrameNumbersRaw(Section, Handles, KeyFrameNumbers);
	FFrameRate DisplayRate = Sequencer->GetMovieScene()->GetDisplayRate();
	FFrameRate TickResolution = Sequencer->GetMovieScene()->GetTickResolution();
	ToSeconds(KeyFrameNumbers, DisplayRate, TickResolution, ESequenceTimeUnit::TickResolution, KeySeconds);
}

void UMovieSceneChannelBPFLib::SetKeyFrameNumberRaw(UMovieSceneSection* Section, FMovieSceneKeyHandle Handle, FFrameNumber KeyFrameNumber)
{
	if (Section == nullptr) { return; }

	FMovieSceneChannel* Channel = GetFirstChannel(Section);
	if (Channel != nullptr)
	{
		Channel->SetKeyTime(*Handle, KeyFrameNumber);
	}
}

void UMovieSceneChannelBPFLib::SetKeyFrameNumbersRaw(UMovieSceneSection* Section, TArray<FMovieSceneKeyHandle> Handles, TArray<FFrameNumber> KeyFrameNumbers)
{
	if (Section == nullptr) { return; }

	FMovieSceneChannel* Channel = GetFirstChannel(Section);
	if (Channel != nullptr)
	{
		TArray<FKeyHandle> KeyHandles;
		ToKeyHandles(Handles, KeyHandles);
		Channel->SetKeyTimes(KeyHandles, KeyFrameNumbers);
	}
}

void UMovieSceneChannelBPFLib::SetKeyFrameNumber(UMovieSceneSequence* Sequencer, UMovieSceneSection* Section, FMovieSceneKeyHandle Handle, ESequenceTimeUnit SequenceTimeUnit, FFrameNumber KeyFrameNumber)
{
	if ((Sequencer == nullptr) || Sequencer->GetMovieScene() == nullptr) { return; }
	if (Section == nullptr) { return; }

	switch (SequenceTimeUnit)
	{
	case ESequenceTimeUnit::DisplayRate:
	{
		FFrameRate DisplayRate = Sequencer->GetMovieScene()->GetDisplayRate();
		FFrameRate TickResolution = Sequencer->GetMovieScene()->GetTickResolution();
		FFrameTime KeyFrameTimes;
		TransformFrame(KeyFrameNumber, DisplayRate, TickResolution, KeyFrameTimes);
		KeyFrameNumber = KeyFrameTimes.FloorToFrame();
	}
	}
	SetKeyFrameNumberRaw(Section, Handle, KeyFrameNumber);
}

void UMovieSceneChannelBPFLib::SetKeyFrameNumbers(UMovieSceneSequence* Sequencer, UMovieSceneSection* Section, TArray<FMovieSceneKeyHandle> Handles, ESequenceTimeUnit SequenceTimeUnit, TArray<FFrameNumber> KeyFrameNumbers)
{
	if ((Sequencer == nullptr) || Sequencer->GetMovieScene() == nullptr) { return; }
	if (Section == nullptr) { return; }

	switch (SequenceTimeUnit)
	{
	case ESequenceTimeUnit::DisplayRate:
	{
		FFrameRate DisplayRate = Sequencer->GetMovieScene()->GetDisplayRate();
		FFrameRate TickResolution = Sequencer->GetMovieScene()->GetTickResolution();
		TArray<FFrameTime> KeyFrameTimes;
		TransformFrames(KeyFrameNumbers, DisplayRate, TickResolution, KeyFrameTimes);
		TimesToFrames(KeyFrameTimes, KeyFrameNumbers);
	}
	}
	SetKeyFrameNumbersRaw(Section, Handles, KeyFrameNumbers);
}

void UMovieSceneChannelBPFLib::SetKeySecond(UMovieSceneSequence* Sequencer, UMovieSceneSection* Section, FMovieSceneKeyHandle Handle, float KeySecond)
{
	if ((Sequencer == nullptr) || Sequencer->GetMovieScene() == nullptr) { return; }
	if (Section == nullptr) { return; }

	FFrameRate DisplayRate = Sequencer->GetMovieScene()->GetDisplayRate();
	FFrameRate TickResolution = Sequencer->GetMovieScene()->GetTickResolution();
	FFrameNumber FrameNumberRaw;
	ToFrameNumber(KeySecond, DisplayRate, TickResolution, ESequenceTimeUnit::TickResolution, FrameNumberRaw);
	SetKeyFrameNumberRaw(Section, Handle, FrameNumberRaw);
}

void UMovieSceneChannelBPFLib::SetKeySeconds(UMovieSceneSequence* Sequencer, UMovieSceneSection* Section, TArray<FMovieSceneKeyHandle> Handles, TArray<float> KeySeconds)
{
	if ((Sequencer == nullptr) || Sequencer->GetMovieScene() == nullptr) { return; }
	if (Section == nullptr) { return; }

	FFrameRate DisplayRate = Sequencer->GetMovieScene()->GetDisplayRate();
	FFrameRate TickResolution = Sequencer->GetMovieScene()->GetTickResolution();
	TArray<FFrameNumber> FrameNumbersRaw;
	ToFrameNumbers(KeySeconds, DisplayRate, TickResolution, ESequenceTimeUnit::TickResolution, FrameNumbersRaw);
	SetKeyFrameNumbersRaw(Section, Handles, FrameNumbersRaw);
}

void UMovieSceneChannelBPFLib::DuplicateKey(UMovieSceneSection* Section, FMovieSceneKeyHandle Handle, FMovieSceneKeyHandle& NewHandle)
{
	if (Section == nullptr) { return; }

	FMovieSceneChannel* Channel = GetFirstChannel(Section);
	if (Channel != nullptr)
	{
		FKeyHandle KeyHandle;
		TArray<FKeyHandle> DuplicatedHandles;
		KeyHandle = *Handle;
		DuplicatedHandles.SetNum(1);
		Channel->DuplicateKeys({ KeyHandle }, DuplicatedHandles);
		NewHandle = DuplicatedHandles[0];
	}
}

void UMovieSceneChannelBPFLib::DuplicateKeys(UMovieSceneSection* Section, TArray<FMovieSceneKeyHandle> Handles, TArray<FMovieSceneKeyHandle>& NewHandles)
{
	if (Section == nullptr) { return; }

	FMovieSceneChannel* Channel = GetFirstChannel(Section);
	if (Channel != nullptr)
	{
		TArray<FKeyHandle> KeyHandles, DuplicatedHandles;
		ToKeyHandles(Handles, KeyHandles);
		DuplicatedHandles.SetNum(KeyHandles.Num());
		Channel->DuplicateKeys(KeyHandles, DuplicatedHandles);
		ToKeyHandles(DuplicatedHandles, NewHandles);
	}
}

void UMovieSceneChannelBPFLib::DeleteKey(UMovieSceneSection* Section, FMovieSceneKeyHandle Handle)
{
	if (Section == nullptr) { return; }

	FMovieSceneChannel* Channel = GetFirstChannel(Section);
	if (Channel != nullptr)
	{
		TArray<FKeyHandle> KeyHandles, NewHandles;
		ToKeyHandles({ Handle }, KeyHandles);
		Channel->DeleteKeys(KeyHandles);
	}
}

void UMovieSceneChannelBPFLib::DeleteKeys(UMovieSceneSection* Section, TArray<FMovieSceneKeyHandle> Handles)
{
	if (Section == nullptr) { return; }

	FMovieSceneChannel* Channel = GetFirstChannel(Section);
	if (Channel != nullptr)
	{
		TArray<FKeyHandle> KeyHandles, NewHandles;
		ToKeyHandles(Handles, KeyHandles);
		Channel->DeleteKeys(KeyHandles);
	}
}

void UMovieSceneChannelBPFLib::DeleteKeysFrom(UMovieSceneSection* Section, FFrameNumber FrameNumber, bool bDeleteKeysBefore)
{
	if (Section == nullptr) { return; }

	FMovieSceneChannel* Channel = GetFirstChannel(Section);
	if (Channel != nullptr)
	{
		Channel->DeleteKeysFrom(FrameNumber, bDeleteKeysBefore);
	}
}

void UMovieSceneChannelBPFLib::ChangeFrameResolution(UMovieSceneSection* Section, FFrameRate SourceRate, FFrameRate DestinationRate)
{
	if (Section == nullptr) { return; }

	FMovieSceneChannel* Channel = GetFirstChannel(Section);
	if (Channel != nullptr)
	{
		Channel->ChangeFrameResolution(SourceRate, DestinationRate);
	}
}

FFrameNumberRange UMovieSceneChannelBPFLib::ComputeEffectiveRange(const UMovieSceneSection* Section)
{
	if (Section == nullptr) { return FFrameNumberRange::Empty(); }

	const FMovieSceneChannel* Channel = GetFirstChannel(Section);
	if (Channel != nullptr)
	{
		Channel->ComputeEffectiveRange();
	}

	return FFrameNumberRange::Empty();
}

void UMovieSceneChannelBPFLib::Reset(UMovieSceneSection* Section)
{
	if (Section == nullptr) { return; }

	FMovieSceneChannel* Channel = GetFirstChannel(Section);
	if (Channel != nullptr)
	{
		Channel->Reset();
	}
}

void UMovieSceneChannelBPFLib::Offset(UMovieSceneSection* Section, FFrameNumber DeltaPosition)
{
	if (Section == nullptr) { return; }

	FMovieSceneChannel* Channel = GetFirstChannel(Section);
	if (Channel != nullptr)
	{
		Channel->Offset(DeltaPosition);
	}
}

void UMovieSceneChannelBPFLib::Optimize(UMovieSceneSection* Section, FFrameNumberRange Range, FFrameRate DisplayRate, float Tolerance, bool bAutoSetInterpolation)
{
	if (Section == nullptr) { return; }

	FMovieSceneChannel* Channel = GetFirstChannel(Section);
	if (Channel != nullptr)
	{
		FKeyDataOptimizationParams Optimization;
		Optimization.Range = Range;
		Optimization.DisplayRate = DisplayRate;
		Optimization.Tolerance = Tolerance;
		Optimization.bAutoSetInterpolation = bAutoSetInterpolation;
		Channel->Optimize(Optimization);
	}
}

void UMovieSceneChannelBPFLib::ClearDefault(UMovieSceneSection* Section)
{
	if (Section == nullptr) { return; }

	FMovieSceneChannel* Channel = GetFirstChannel(Section);
	if (Channel != nullptr)
	{
		Channel->ClearDefault();
	}
}

void UMovieSceneChannelBPFLib::PostEditChange(UMovieSceneSection* Section)
{
	if (Section == nullptr) { return; }

	FMovieSceneChannel* Channel = GetFirstChannel(Section);
	if (Channel != nullptr)
	{
		Channel->PostEditChange();
	}
}
