#include "Launcher.h"


LauncherClass::LauncherClass()
{
	LaunchTrig = false;
	prevLaunchTrig = false;

	currentTestBTN = 0;
	prevTestBTN = 0;
	
	LaunchOut = new Solenoid(1,3);
	LaunchIn = new Solenoid(1,4);
			
	StopperOut = new Solenoid(1,5);
	StopperIn = new Solenoid(1,6);
	
	Launch_DelayTM = new Timer();
	LaunchState = "";
	BlockState = "";
	
}

void LauncherClass::Launch()
{
	Launch_DelayTM->Reset();
	Launch_DelayTM->Start();
	LaunchOut->Set(true);
	LaunchIn->Set(false);
}
void LauncherClass::FullLaunch()
{
	Launch();
	while(Launch_DelayTM->Get() < .125)
	{
				
	}
	LaunchOut->Set(false);	
	LaunchIn->Set(true);
}
void LauncherClass::UpdateLaunch_Operator(bool EnableTrig, int TestBTN)//turretstickgettrig, turretstickgetbtn6
{
	prevLaunchTrig = LaunchTrig;
	LaunchTrig = EnableTrig;
	
	prevTestBTN = currentTestBTN;
	currentTestBTN = TestBTN;
		
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
			LaunchIn->Set(true);
			StopperOut->Set(true);
			StopperIn->Set(false);
		}
		
		if((LaunchIn->Get() == true)&&(LaunchOut->Get() == false))
		{
			LaunchState = "In";
		}
		if((LaunchIn->Get() == false)&&(LaunchOut->Get() == true))
		{
			LaunchState = "Out";
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
	Dash->PutString("Launcher",LaunchState);
}





