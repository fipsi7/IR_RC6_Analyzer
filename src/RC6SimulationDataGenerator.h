#ifndef RC6_SIMULATION_DATA_GENERATOR
#define RC6_SIMULATION_DATA_GENERATOR

#include <SimulationChannelDescriptor.h>
#include <string>
class RC6AnalyzerSettings;

class RC6SimulationDataGenerator
{
public:
	RC6SimulationDataGenerator();
	~RC6SimulationDataGenerator();

	void Initialize( U32 simulation_sample_rate, RC6AnalyzerSettings* settings );
	U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channel );

protected:
	RC6AnalyzerSettings* mSettings;
	U32 mSimulationSampleRateHz;

protected:
	void CreateSerialByte();
	std::string mSerialText;
	U32 mStringIndex;

	SimulationChannelDescriptor mSerialSimulationData;

};
#endif //RC6_SIMULATION_DATA_GENERATOR