#ifndef RC6_ANALYZER_H
#define RC6_ANALYZER_H

#include <Analyzer.h>
#include "RC6AnalyzerResults.h"
#include "RC6SimulationDataGenerator.h"

class RC6AnalyzerSettings;
class ANALYZER_EXPORT RC6Analyzer : public Analyzer2
{
public:
	RC6Analyzer();
	virtual ~RC6Analyzer();

	virtual void SetupResults();
	virtual void WorkerThread();

	virtual U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channels );
	virtual U32 GetMinimumSampleRateHz();

	virtual const char* GetAnalyzerName() const;
	virtual bool NeedsRerun();

protected: //vars
	std::auto_ptr< RC6AnalyzerSettings > mSettings;
	std::auto_ptr< RC6AnalyzerResults > mResults;
	AnalyzerChannelData* mSerial;

	RC6SimulationDataGenerator mSimulationDataGenerator;
	bool mSimulationInitilized;

	//Serial analysis vars:
	U32 mSampleRateHz;
	U32 mStartOfStopBitOffset;
	U32 mEndOfStopBitOffset;
};

extern "C" ANALYZER_EXPORT const char* __cdecl GetAnalyzerName();
extern "C" ANALYZER_EXPORT Analyzer* __cdecl CreateAnalyzer( );
extern "C" ANALYZER_EXPORT void __cdecl DestroyAnalyzer( Analyzer* analyzer );

#endif //RC6_ANALYZER_H
