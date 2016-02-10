#ifndef LAUNCHER_H
#define LAUNCHER_H
#include "WPIlib.h"
#include "Ace_Defines.h"
class LauncherClass
{
	bool LaunchTrig;
	bool prevLaunchTrig;
	
	int currentTestBTN;
	int prevTestBTN;
	
	Solenoid *StopperIn;
	Solenoid *StopperOut;
		
	Solenoid *LaunchOut;
#if PRACTICE_BOT
	Solenoid *LaunchIn;
#endif
	Timer *Launch_DelayTM;
	
	DigitalInput *LightSensor;
	
	string LaunchState;
	string BlockState;
	
public:
	bool HoldingBall;
		
	LauncherClass();
	~LauncherClass();
	
	void Launch();
	void FullLaunch();
	void UpdateLaunch_Operator(bool EnableTrig, int TestBTN);
	void UpdateBlock_AUTO();
	void ResetTimers();
	void SendData();
};
#endif
