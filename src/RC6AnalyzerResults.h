#ifndef RC6_ANALYZER_RESULTS
#define RC6_ANALYZER_RESULTS

#include <AnalyzerResults.h>

class RC6Analyzer;
class RC6AnalyzerSettings;

class RC6AnalyzerResults : public AnalyzerResults
{
public:
	RC6AnalyzerResults( RC6Analyzer* analyzer, RC6AnalyzerSettings* settings );
	virtual ~RC6AnalyzerResults();

	virtual void GenerateBubbleText( U64 frame_index, Channel& channel, DisplayBase display_base );
	virtual void GenerateExportFile( const char* file, DisplayBase display_base, U32 export_type_user_id );

	virtual void GenerateFrameTabularText(U64 frame_index, DisplayBase display_base );
	virtual void GeneratePacketTabularText( U64 packet_id, DisplayBase display_base );
	virtual void GenerateTransactionTabularText( U64 transaction_id, DisplayBase display_base );

protected: //functions

protected:  //vars
	RC6AnalyzerSettings* mSettings;
	RC6Analyzer* mAnalyzer;
};

#endif //RC6_ANALYZER_RESULTS
