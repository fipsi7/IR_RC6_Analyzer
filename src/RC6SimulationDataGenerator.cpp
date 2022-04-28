#include "RC6SimulationDataGenerator.h"
#include "RC6AnalyzerSettings.h"

#include <AnalyzerHelpers.h>

RC6SimulationDataGenerator::RC6SimulationDataGenerator()
:	mAddress( 0xFF ),
	mData( 0xFF )
{
}

RC6SimulationDataGenerator::~RC6SimulationDataGenerator()
{
}

void RC6SimulationDataGenerator::Initialize( U32 simulation_sample_rate, RC6AnalyzerSettings* settings )
{
	mSimulationSampleRateHz = simulation_sample_rate;
	mSettings = settings;

	mSerialSimulationData.SetChannel( mSettings->mInputChannel );
	mSerialSimulationData.SetSampleRate( simulation_sample_rate );
	mSerialSimulationData.SetInitialBitState( BIT_LOW );
}

U32 RC6SimulationDataGenerator::GenerateSimulationData( U64 largest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channel )
{
	U64 adjusted_largest_sample_requested = AnalyzerHelpers::AdjustSimulationTargetSample( largest_sample_requested, sample_rate, mSimulationSampleRateHz );

	while( mSerialSimulationData.GetCurrentSampleNumber() < adjusted_largest_sample_requested )
	{
		CreateSerialByte();
	}

	*simulation_channel = &mSerialSimulationData;
	return 1;
}

void RC6SimulationDataGenerator::CreateSerialByte()
{
	U32 samples_per_bit = mSimulationSampleRateHz / (mSettings->mCarrierFrequency/16); //Bit Frequency 16 times slower as the carrier frequency

	//let's move forward a little
	mSerialSimulationData.Advance( samples_per_bit * 10 );

	//start symbol
	mSerialSimulationData.TransitionIfNeeded( BIT_HIGH );
	mSerialSimulationData.Advance( samples_per_bit * 6 );
	mSerialSimulationData.TransitionIfNeeded( BIT_LOW );
	mSerialSimulationData.Advance( samples_per_bit * 2 );

	//first bit
	mSerialSimulationData.TransitionIfNeeded( BIT_HIGH );
	mSerialSimulationData.Advance( samples_per_bit );
	mSerialSimulationData.TransitionIfNeeded( BIT_LOW );
	mSerialSimulationData.Advance( samples_per_bit );

	//Mode Bits - Mode 0
	mSerialSimulationData.TransitionIfNeeded( BIT_LOW );
	mSerialSimulationData.Advance( samples_per_bit );
	mSerialSimulationData.TransitionIfNeeded( BIT_HIGH );
	mSerialSimulationData.Advance( samples_per_bit );
	mSerialSimulationData.TransitionIfNeeded( BIT_LOW );
	mSerialSimulationData.Advance( samples_per_bit );
	mSerialSimulationData.TransitionIfNeeded( BIT_HIGH );
	mSerialSimulationData.Advance( samples_per_bit );
	mSerialSimulationData.TransitionIfNeeded( BIT_LOW );
	mSerialSimulationData.Advance( samples_per_bit );
	mSerialSimulationData.TransitionIfNeeded( BIT_HIGH );
	mSerialSimulationData.Advance( samples_per_bit );

	//TOGGLE - TODO
	mSerialSimulationData.TransitionIfNeeded( BIT_LOW );
	mSerialSimulationData.Advance( samples_per_bit * 2);
	mSerialSimulationData.TransitionIfNeeded( BIT_HIGH );
	mSerialSimulationData.Advance( samples_per_bit * 2);

	
	// ADDRESS
	U8 mask = 0x1 << 7;
	for( U32 i=0; i<8; i++ )
	{
		if( ( mAddress & mask ) != 0 ){
			mSerialSimulationData.TransitionIfNeeded( BIT_HIGH );
			mSerialSimulationData.Advance( samples_per_bit );
			mSerialSimulationData.TransitionIfNeeded( BIT_LOW );
		} else{
			mSerialSimulationData.TransitionIfNeeded( BIT_LOW );
			mSerialSimulationData.Advance( samples_per_bit );
			mSerialSimulationData.TransitionIfNeeded( BIT_HIGH );
		}

		mSerialSimulationData.Advance( samples_per_bit );
		mask = mask >> 1;
	}

	// DATA
	mask = 0x1 << 7;
	for( U32 i=0; i<8; i++ )
	{
		if( ( mData & mask ) != 0 ){
			mSerialSimulationData.TransitionIfNeeded( BIT_HIGH );
			mSerialSimulationData.Advance( samples_per_bit );
			mSerialSimulationData.TransitionIfNeeded( BIT_LOW );
		} else{
			mSerialSimulationData.TransitionIfNeeded( BIT_LOW );
			mSerialSimulationData.Advance( samples_per_bit );
			mSerialSimulationData.TransitionIfNeeded( BIT_HIGH );
		}

		mSerialSimulationData.Advance( samples_per_bit );
		mask = mask >> 1;
	}

	mSerialSimulationData.TransitionIfNeeded( BIT_LOW ); //we need to end low

	//lets pad the end a bit for the stop bit:
	mSerialSimulationData.Advance( samples_per_bit * 10);
}
