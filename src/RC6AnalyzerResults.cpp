#include "RC6AnalyzerResults.h"
#include <AnalyzerHelpers.h>
#include "RC6Analyzer.h"
#include "RC6AnalyzerSettings.h"
#include <iostream>
#include <fstream>

RC6AnalyzerResults::RC6AnalyzerResults( RC6Analyzer* analyzer, RC6AnalyzerSettings* settings )
:	AnalyzerResults(),
	mSettings( settings ),
	mAnalyzer( analyzer )
{
}

RC6AnalyzerResults::~RC6AnalyzerResults()
{
}

void RC6AnalyzerResults::GenerateBubbleText( U64 frame_index, Channel& channel, DisplayBase display_base )
{
	ClearResultStrings();
	Frame frame = GetFrame( frame_index );

	char number_str[128];
	AnalyzerHelpers::GetNumberString( frame.mData1, display_base, 8, number_str, 128 );
	if(frame.HasFlag(1)){
		AddResultString( "LEADING PREAMBLE" );
	}else if(frame.HasFlag(2)){
		AddResultString( "ONE BIT" );
	}else if(frame.HasFlag(4)){
		AddResultString( "Mode:" , number_str );
	}else if(frame.HasFlag(8)){
		AddResultString( "Toggle:" , number_str );
	}else if(frame.HasFlag(16)){
		AddResultString( "Address:" , number_str );
	}else if(frame.HasFlag(32)){
		AddResultString( "Data:" , number_str );
	}else{
		AddResultString( number_str );
	}
}

void RC6AnalyzerResults::GenerateExportFile( const char* file, DisplayBase display_base, U32 export_type_user_id )
{
	std::ofstream file_stream( file, std::ios::out );

	U64 trigger_sample = mAnalyzer->GetTriggerSample();
	U32 sample_rate = mAnalyzer->GetSampleRate();

	file_stream << "Time [s],Value" << std::endl;

	U64 num_frames = GetNumFrames();
	for( U32 i=0; i < num_frames; i++ )
	{
		Frame frame = GetFrame( i );
		
		char time_str[128];
		AnalyzerHelpers::GetTimeString( frame.mStartingSampleInclusive, trigger_sample, sample_rate, time_str, 128 );

		char number_str[128];
		AnalyzerHelpers::GetNumberString( frame.mData1, display_base, 8, number_str, 128 );

		file_stream << time_str << "," << number_str << std::endl;

		if( UpdateExportProgressAndCheckForCancel( i, num_frames ) == true )
		{
			file_stream.close();
			return;
		}
	}

	file_stream.close();
}

void RC6AnalyzerResults::GenerateFrameTabularText( U64 frame_index, DisplayBase display_base )
{
#ifdef SUPPORTS_PROTOCOL_SEARCH
	Frame frame = GetFrame( frame_index );
	ClearTabularText();

	char number_str[128];
	AnalyzerHelpers::GetNumberString( frame.mData1, display_base, 8, number_str, 128 );
	if(frame.HasFlag(1)){
		AddTabularText( "LEADING PREAMBLE" );
	}else if(frame.HasFlag(2)){
		AddTabularText( "ONE BIT" );
	}else if(frame.HasFlag(4)){
		AddTabularText( "Mode:" , number_str );
	}else if(frame.HasFlag(8)){
		AddTabularText( "Toggle:" , number_str );
	}else if(frame.HasFlag(16)){
		AddTabularText( "Address:" , number_str );
	}else if(frame.HasFlag(32)){
		AddTabularText( "Data:" , number_str );
	}else{
		AddTabularText( number_str );
	}
#endif
}

void RC6AnalyzerResults::GeneratePacketTabularText( U64 packet_id, DisplayBase display_base )
{
	//not supported

}

void RC6AnalyzerResults::GenerateTransactionTabularText( U64 transaction_id, DisplayBase display_base )
{
	//not supported
}