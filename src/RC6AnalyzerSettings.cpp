#include "RC6AnalyzerSettings.h"
#include <AnalyzerHelpers.h>


RC6AnalyzerSettings::RC6AnalyzerSettings()
:	mInputChannel( UNDEFINED_CHANNEL ),
	mCarrierFrequency( 36000 )
{
	mInputChannelInterface.reset( new AnalyzerSettingInterfaceChannel() );
	mInputChannelInterface->SetTitleAndTooltip( "IR Pin", "Standard Vogt's IR RC6 Analyzer " );
	mInputChannelInterface->SetChannel( mInputChannel );

	mCarrierFrequencyInterface.reset( new AnalyzerSettingInterfaceInteger() );
	mCarrierFrequencyInterface->SetTitleAndTooltip( "Carrier Frequency (Hz)",  "Specify thecarrier frequency in Hz." );
	mCarrierFrequencyInterface->SetMax( 6000000 );
	mCarrierFrequencyInterface->SetMin( 1 );
	mCarrierFrequencyInterface->SetInteger( mCarrierFrequency );

	AddInterface( mInputChannelInterface.get() );
	AddInterface( mCarrierFrequencyInterface.get() );

	AddExportOption( 0, "Export as text/csv file" );
	AddExportExtension( 0, "text", "txt" );
	AddExportExtension( 0, "csv", "csv" );

	ClearChannels();
	AddChannel( mInputChannel, "Serial", false );
}

RC6AnalyzerSettings::~RC6AnalyzerSettings()
{
}

bool RC6AnalyzerSettings::SetSettingsFromInterfaces()
{
	mInputChannel = mInputChannelInterface->GetChannel();
	mCarrierFrequency = mCarrierFrequencyInterface->GetInteger();

	ClearChannels();
	AddChannel( mInputChannel, "Vogt's IR RC6 Analyzer ", true );

	return true;
}

void RC6AnalyzerSettings::UpdateInterfacesFromSettings()
{
	mInputChannelInterface->SetChannel( mInputChannel );
	mCarrierFrequencyInterface->SetInteger( mCarrierFrequency );
}

void RC6AnalyzerSettings::LoadSettings( const char* settings )
{
	SimpleArchive text_archive;
	text_archive.SetString( settings );

	text_archive >> mInputChannel;
	text_archive >> mCarrierFrequency;

	ClearChannels();
	AddChannel( mInputChannel, "Vogt's IR RC6 Analyzer ", true );

	UpdateInterfacesFromSettings();
}

const char* RC6AnalyzerSettings::SaveSettings()
{
	SimpleArchive text_archive;

	text_archive << mInputChannel;
	text_archive << mCarrierFrequency;

	return SetReturnString( text_archive.GetString() );
}
