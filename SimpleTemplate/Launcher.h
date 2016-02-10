#include "WPIlib.h"

class LauncherClass
{
	
	bool LaunchTrig;
	bool prevLaunchTrig;
	
	int currentTestBTN;
	int prevTestBTN;
	
	Solenoid *StopperIn;
	Solenoid *StopperOut;
		
	Solenoid *LaunchOut;
	Solenoid *LaunchIn;
	
	Timer *Launch_DelayTM;
	
	string LaunchState;
	string BlockState;
	
public:
	
		
	LauncherClass();
	~LauncherClass();
	
	void Launch();
	void FullLaunch();
	void UpdateLaunch_Operator(bool EnableTrig, int TestBTN);
	void ResetTimers();
	void SendData();
};
