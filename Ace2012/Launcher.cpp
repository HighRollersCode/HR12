#include "Launcher.h"

LauncherClass::LauncherClass()
{
	LaunchTrig = false;
	prevLaunchTrig = false;

	currentTestBTN = 0;
	prevTestBTN = 0;
	
	LaunchOut = new Solenoid(1,3);
#if PRACTICE_BOT
	LaunchIn = new Solenoid(1,4);
#endif		
	StopperOut = new Solenoid(1,5);
	StopperIn = new Solenoid(1,6);
	
	LightSensor = new DigitalInput(11);
	
	Launch_DelayTM = new Timer();
	LaunchState = "";
	BlockState = "";
	
	HoldingBall = false;
	
}
void LauncherClass::Launch()
{
	Launch_DelayTM->Reset();
	Launch_DelayTM->Start();
	LaunchOut->Set(true);
#if PRACTICE_BOT
	LaunchIn->Set(false);
#endif
}
void LauncherClass::FullLaunch()
{
	Launch();
	while(Launch_DelayTM->Get() < .125)
	{
				
	}
	LaunchOut->Set(false);	
#if PRACTICE_BOT
	LaunchIn->Set(true);
#endif
	StopperOut->Set(true);
	StopperIn->Set(false);
}
void LauncherClass::UpdateLaunch_Operator(bool EnableTrig, int TestBTN)//turretstickgettrig, turretstickgetbtn6
{
	prevLaunchTrig = LaunchTrig;
	LaunchTrig = EnableTrig;
	
	prevTestBTN = currentTestBTN;
	currentTestBTN = TestBTN;
//	LaunchOut->Set(EnableTrig);
		
	if((currentTestBTN == 1)&&(prevTestBTN == 0))
		{
			Launch_DelayTM->Reset();
			Launch_DelayTM->Stop();
			StopperIn->Set(true);
			StopperOut->Set(false);
		}
		if((LaunchTrig == 1)&& (prevLaunchTrig == 0))
		{
			Launch();
		}
		
		if(Launch_DelayTM->Get() > .125)
		{
			LaunchOut->Set(false);	
#if PRACTICE_BOT
			LaunchIn->Set(true);
#endif
			StopperOut->Set(true);
			StopperIn->Set(false);
		}
		
	//	if((LaunchIn->Get() == true)&&(LaunchOut->Get() == false))
	//	{
	//		LaunchState = "In";
	//	}
	//	if((LaunchIn->Get() == false)&&(LaunchOut->Get() == true))
	//	{
	//		LaunchState = "Out";
	//	}
		if(LightSensor->Get() == 1)
		{
		//	StopperIn->Set(true);
		//	StopperOut->Set(false);
		}
		
}
void LauncherClass::UpdateBlock_AUTO()
{
	if(LightSensor->Get() == 1)
	{
		StopperIn->Set(true);
		StopperOut->Set(false);
		HoldingBall = true;
	}
	else
	{
		HoldingBall = false; // JUST ADDED WILL FIX AUTO
	}
}
void LauncherClass::ResetTimers()
{
	Launch_DelayTM->Stop();
	Launch_DelayTM->Reset();
}
void LauncherClass::SendData()
{
	SmartDashboard *Dash = SmartDashboard::GetInstance();
	Dash->PutInt("Light",LightSensor->Get());
	//Dash->PutBoolean("launcher",LaunchOut->Get());
	//Dash->PutString("Launcher",LaunchState);
}


