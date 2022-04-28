#include "RC6Analyzer.h"
#include "RC6AnalyzerSettings.h"
#include <AnalyzerChannelData.h>

RC6Analyzer::RC6Analyzer()
:	Analyzer2(),  
	mSettings( new RC6AnalyzerSettings() ),
	mSimulationInitilized( false )
{
	SetAnalyzerSettings( mSettings.get() );
}

RC6Analyzer::~RC6Analyzer()
{
	KillThread();
}

void RC6Analyzer::SetupResults()
{
	mResults.reset( new RC6AnalyzerResults( this, mSettings.get() ) );
	SetAnalyzerResults( mResults.get() );
	mResults->AddChannelBubblesWillAppearOn( mSettings->mInputChannel );
}

void RC6Analyzer::WorkerThread()
{
	mSampleRateHz = GetSampleRate();

	mSerial = GetAnalyzerChannelData( mSettings->mInputChannel );

	//if( mSerial->GetBitState() == BIT_LOW )
	//	mSerial->AdvanceToNextEdge();

	U32 samples_per_bit = mSampleRateHz /(mSettings->mCarrierFrequency/16);

	for( ; ; )
	{
		U8 address = 0;
		U8 data = 0;
		U8 mode = 0;
		U8 toggle = 0;
		U8 state = 0;
		U64 starting_sample = 0;
		for( ; ; ){
			mSerial->AdvanceToNextEdge(); 
			starting_sample = mSerial->GetSampleNumber();
			mSerial->Advance( samples_per_bit >> 1 );
			
			for(int i = 0; i < 6; i++){
				mResults->AddMarker( mSerial->GetSampleNumber(), AnalyzerResults::Square, mSettings->mInputChannel );
				if( mSerial->GetBitState() == BIT_HIGH ){
					state++;
				}
					mSerial->Advance( samples_per_bit );
			}
			for(int i = 0; i < 2; i++){
				mResults->AddMarker( mSerial->GetSampleNumber(), AnalyzerResults::Square, mSettings->mInputChannel );
				if( mSerial->GetBitState() == BIT_LOW ){
					state++;
				}
					mSerial->Advance( samples_per_bit );
			}
			if(state == 8){
				Frame frame;
				frame.mData1 = data;
				frame.mFlags = 1;
				frame.mStartingSampleInclusive = starting_sample;
				frame.mEndingSampleInclusive = mSerial->GetSampleNumber() - (samples_per_bit >> 1);

				mResults->AddFrame( frame );
				mResults->CommitResults();
				ReportProgress( frame.mEndingSampleInclusive );
			}else{
				continue;
			}
			starting_sample = mSerial->GetSampleNumber();
			mResults->AddMarker( mSerial->GetSampleNumber(), AnalyzerResults::Square, mSettings->mInputChannel );
			if( mSerial->GetBitState() == BIT_HIGH ){
				state++;
			}
			mSerial->Advance( samples_per_bit );

			mResults->AddMarker( mSerial->GetSampleNumber(), AnalyzerResults::Square, mSettings->mInputChannel );
			if( mSerial->GetBitState() == BIT_LOW ){
				state++;
			}
			mSerial->Advance( samples_per_bit );
			if(state == 10){
				Frame frame;
				frame.mData1 = data;
				frame.mFlags = 2;
				frame.mStartingSampleInclusive = starting_sample - (samples_per_bit >> 1);
				frame.mEndingSampleInclusive = mSerial->GetSampleNumber() - (samples_per_bit >> 1);

				mResults->AddFrame( frame );
				mResults->CommitResults();
				ReportProgress( frame.mEndingSampleInclusive );
				break;
			}
		}
		starting_sample = mSerial->GetSampleNumber();
		U8 mask = 1 << 2;
		for( U32 i=0; i<3; i++ )
		{
			mResults->AddMarker( mSerial->GetSampleNumber(), AnalyzerResults::Dot, mSettings->mInputChannel );

			if( mSerial->GetBitState() == BIT_HIGH )
				mode |= mask;

			mSerial->Advance( samples_per_bit );
			mResults->AddMarker( mSerial->GetSampleNumber(), AnalyzerResults::Dot, mSettings->mInputChannel );
			// TODO check, if second bit is complement of first
			mSerial->Advance( samples_per_bit );

			mask = mask >> 1;
		}

		Frame frame;
		frame.mData1 = mode;
		frame.mFlags = 4;
		frame.mStartingSampleInclusive = starting_sample - (samples_per_bit >> 1);
		frame.mEndingSampleInclusive = mSerial->GetSampleNumber() - (samples_per_bit >> 1);

		mResults->AddFrame( frame );
		mResults->CommitResults();
		ReportProgress( frame.mEndingSampleInclusive );

		// TOGGLE
		starting_sample = mSerial->GetSampleNumber();

		mResults->AddMarker( mSerial->GetSampleNumber(), AnalyzerResults::Dot, mSettings->mInputChannel );

		if( mSerial->GetBitState() == BIT_HIGH )
			toggle = 1;
		mSerial->Advance( samples_per_bit );
		mResults->AddMarker( mSerial->GetSampleNumber(), AnalyzerResults::Dot, mSettings->mInputChannel );
		mSerial->Advance( samples_per_bit );
		mResults->AddMarker( mSerial->GetSampleNumber(), AnalyzerResults::Dot, mSettings->mInputChannel );
		mSerial->Advance( samples_per_bit );
		mResults->AddMarker( mSerial->GetSampleNumber(), AnalyzerResults::Dot, mSettings->mInputChannel );
		mSerial->Advance( samples_per_bit );

		frame.mData1 = toggle;
		frame.mFlags = 8;
		frame.mStartingSampleInclusive = starting_sample - (samples_per_bit >> 1);
		frame.mEndingSampleInclusive = mSerial->GetSampleNumber() - (samples_per_bit >> 1);

		mResults->AddFrame( frame );
		mResults->CommitResults();
		ReportProgress( frame.mEndingSampleInclusive );

		starting_sample = mSerial->GetSampleNumber();
		mask = 1 << 7;
		for( U32 i=0; i<8; i++ )
		{
			//let's put a dot exactly where we sample this bit:
			mResults->AddMarker( mSerial->GetSampleNumber(), AnalyzerResults::Dot, mSettings->mInputChannel );

			if( mSerial->GetBitState() == BIT_HIGH )
				address |= mask;

			mSerial->Advance( samples_per_bit );
			mResults->AddMarker( mSerial->GetSampleNumber(), AnalyzerResults::Dot, mSettings->mInputChannel );
			mSerial->Advance( samples_per_bit );

			mask = mask >> 1;
		}

		frame.mData1 = address;
		frame.mFlags = 16;
		frame.mStartingSampleInclusive = starting_sample - (samples_per_bit >> 1);
		frame.mEndingSampleInclusive = mSerial->GetSampleNumber() - (samples_per_bit >> 1);

		mResults->AddFrame( frame );
		mResults->CommitResults();
		ReportProgress( frame.mEndingSampleInclusive );


		starting_sample = mSerial->GetSampleNumber();
		mask = 1 << 7;
		for( U32 i=0; i<8; i++ )
		{
			//let's put a dot exactly where we sample this bit:
			mResults->AddMarker( mSerial->GetSampleNumber(), AnalyzerResults::Dot, mSettings->mInputChannel );

			if( mSerial->GetBitState() == BIT_HIGH )
				data |= mask;

			mSerial->Advance( samples_per_bit );
			mResults->AddMarker( mSerial->GetSampleNumber(), AnalyzerResults::Dot, mSettings->mInputChannel );
			mSerial->Advance( samples_per_bit );

			mask = mask >> 1;
		}

		frame.mData1 = data;
		frame.mFlags = 32;
		frame.mStartingSampleInclusive = starting_sample - (samples_per_bit >> 1);
		frame.mEndingSampleInclusive = mSerial->GetSampleNumber() - (samples_per_bit >> 1);

		mResults->AddFrame( frame );
		mResults->CommitResults();
		ReportProgress( frame.mEndingSampleInclusive );
	}
}

bool RC6Analyzer::NeedsRerun()
{
	return false;
}

U32 RC6Analyzer::GenerateSimulationData( U64 minimum_sample_index, U32 device_sample_rate, SimulationChannelDescriptor** simulation_channels )
{
	if( mSimulationInitilized == false )
	{
		mSimulationDataGenerator.Initialize( GetSimulationSampleRate(), mSettings.get() );
		mSimulationInitilized = true;
	}

	return mSimulationDataGenerator.GenerateSimulationData( minimum_sample_index, device_sample_rate, simulation_channels );
}

U32 RC6Analyzer::GetMinimumSampleRateHz()
{
	return mSettings->mCarrierFrequency * 4;
}

const char* RC6Analyzer::GetAnalyzerName() const
{
	return "Vogt's IR RC6 Analyzer ";
}

const char* GetAnalyzerName()
{
	return "Vogt's IR RC6 Analyzer ";
}

Analyzer* CreateAnalyzer()
{
	return new RC6Analyzer();
}

void DestroyAnalyzer( Analyzer* analyzer )
{
	delete analyzer;
}