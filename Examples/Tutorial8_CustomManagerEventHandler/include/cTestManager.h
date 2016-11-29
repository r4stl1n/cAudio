//****************************************************************
//cAudio 2.3.0 Tutorial 8
//Custom Manager Event Handler
//****************************************************************


///Include the ISourceHandler.h so we can inherit the interface
#include "IManagerEventHandler.h"
//Include cAudio.h so we can work with cAudio
#include "cAudio.h"

class cTestManager : public cAudio::IManagerEventHandler
{
	//In order for this handler to work it must have each of the following functions
public:
    cTestManager();
    ~cTestManager();

	//functions required to inherit from cAudio::IManagerEventHandler
    void onInit() {};
    void onUpdate();
    void onRelease() {};
    void onSourceCreate() {};
    void onDecoderRegister() {};
    void onDataSourceRegister() {};
    
private:
    cAudio::IAudioManager* audioMgr;
    cAudio::IAudioSource* mysound;
    bool mysoundFading;
	clock_t fadeClockLastVal;
	float fadeAccumulator;    

};
