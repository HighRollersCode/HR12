
#include "DriveTrain.h"
#include "Ace_Defines.h"
#include <math.h>
DriveTrainClass::DriveTrainClass()
{
	LeftSideTicks = 0;
	RightSideTicks = 0;
	EncoderFailCount = 0;
	TickingEncoders = 1;
	PrevLEnc = 0;
	PrevREnc = 0;
	current_AutonTicks = 0;
	
	RightDrive = new Victor(1);//1
	RightDrive_a = new Victor(2);//2
	LeftDrive = new Victor(3); //3
	LeftDrive_a = new Victor(4);//4
	LeftDriveEnc = new Encoder(1,2);
	RightDriveEnc = new Encoder(3,4);
	
	ShifterHigh = new Solenoid(1,1);
	ShifterLow = new Solenoid(1,2);
	//NeutralHigh = new Solenoid(1,7);
	//NeutralLow = new Solenoid(1,8);
	Neutral_DelayTM = new Timer();

	CurrentBrakeBTN = 0;
	PrevBrakeBTN = 0;
	
	CurrentShifterToggleTrig = false;
	PrevShifterToggleTrig = false;
	
	NeutralBut = false;
	PrevNeutralBut = false;
	neutral = false;
	was_neutral = false;
	ToggleState = 1;
	
	Highgear = false;
	Lowgear = false;

	gear = "";
}

void DriveTrainClass::UpdateEBrake(int enable,int targ)
{

	PrevBrakeBTN = CurrentBrakeBTN;
	CurrentBrakeBTN = enable;
	if((CurrentBrakeBTN == 1)&&(PrevBrakeBTN == 0))
	{
		LeftDriveEnc->Reset();
		RightDriveEnc->Reset();
		LeftDriveEnc->Start();
		RightDriveEnc->Start();
	}
	//rightstickbtn11
	if(enable == 1)
	{
		LeftSideTicks = LeftDriveEnc->Get();
#if PRACTICE_BOT 
		RightSideTicks = (int)(2.5*RightDriveEnc->Get());
#else  
		RightSideTicks = RightDriveEnc->Get();
#endif
	
		float Lerror = LeftSideTicks-targ;
		float Rerror = RightSideTicks+targ;
	
		float Leftout = Lerror*.015f;
		float Rightout = -Rerror*.015f;
		
		LeftDrive->Set(Leftout);
		LeftDrive_a->Set(Leftout);
		
		RightDrive->Set(-Rightout);
		RightDrive_a->Set(-Rightout);
	}
	if((CurrentBrakeBTN == 0) && (PrevBrakeBTN == 1))
	{
		LeftDriveEnc->Reset();
		RightDriveEnc->Reset();
		LeftDriveEnc->Stop();
		RightDriveEnc->Stop();
	}
}
void DriveTrainClass::Drive_Auton(float Forward, float Turn)
{			
	LeftDrive->Set(-Forward-Turn);
	RightDrive->Set(Forward-Turn);
	
	LeftDrive_a->Set(-Forward-Turn);
	RightDrive_a->Set(Forward-Turn);
}
void DriveTrainClass::Drive_Operator(float lY, float rY)
{
	LeftDrive->Set(-lY);//66
	LeftDrive_a->Set(-lY);
	RightDrive->Set(rY);
	RightDrive_a->Set(rY);
}
void DriveTrainClass::ResetEncoders_Timers()
{
	RightDriveEnc->Reset();
	RightDriveEnc->Start();
	LeftDriveEnc->Reset();
	LeftDriveEnc->Start();
	Neutral_DelayTM->Stop();
	Neutral_DelayTM->Reset();
}
void DriveTrainClass::Shifter_Update(bool ShifterEnable,int Neutralbtn)//rightstickgettrig,rightstickbtn10
{
	PrevShifterToggleTrig = CurrentShifterToggleTrig;
	PrevNeutralBut = NeutralBut;
	CurrentShifterToggleTrig = ShifterEnable;
	NeutralBut = Neutralbtn;
	
	was_neutral= neutral;
	
	if(PrevShifterToggleTrig == false && CurrentShifterToggleTrig == true)
	{
		//Neutral_DelayTM->Reset();
		//Neutral_DelayTM->Start();
		//if(neutral == false)
		//{
			ToggleState = -ToggleState;
		//}
		//else
		//{
		//	neutral = false;
		//	ToggleState = 1;
		//}
	}
//	if(PrevNeutralBut == false && NeutralBut == true)
	//{
		//neutral = true;
		//Neutral_DelayTM->Reset();
		//Neutral_DelayTM->Start();
	//}
	//if(neutral == true)
	//{
		//ShifterHigh->Set(false);
		//ShifterLow->Set(true);
		//if(Neutral_DelayTM->Get() > .250)
		//{
			//NeutralHigh->Set(false);
			//NeutralLow->Set(true);		
		//}
		
	//}
	else if(ToggleState == 1)
	{
		//NeutralHigh->Set(true);
		//NeutralLow->Set(false);
		//if(Neutral_DelayTM->Get() > .250)
		//{
			ShifterHigh->Set(true);
			ShifterLow->Set(false);
		//}
	}
	else if(ToggleState == -1)
	{
		//NeutralHigh->Set(true);
		//NeutralLow->Set(false);
		//if(Neutral_DelayTM->Get() > .250)
		//{
			ShifterHigh->Set(false);
			ShifterLow->Set(true);
		//}
	}
	if((ShifterHigh->Get() == false)&&(ShifterLow->Get() == true))
	{
		Lowgear = true;
		Highgear = false;
	}
	if((ShifterHigh->Get() == true)&&(ShifterLow->Get() == false))
	{
		Lowgear = false;
		Highgear = true;
	}
	if(Lowgear)
	{
		gear = "Low Gear";
	}
	if(Highgear)
	{
		gear = "High Gear";
	}
}
void DriveTrainClass::SendData()
{
	SmartDashboard *Dash = SmartDashboard::GetInstance();
	Dash->PutInt("Left Drive Encocder",LeftDriveEnc->Get());
#if PRACTICE_BOT
    Dash->PutInt("Right Drive Encocder",(int)(2.5*RightDriveEnc->Get()));
#else
    Dash->PutInt("Right Drive Encocder",(int)(RightDriveEnc->Get()));
#endif
	Dash->PutDouble("Left Drive Command",(double)LeftDrive_a->Get());
	Dash->PutDouble("Right Drive Command",(double)RightDrive_a->Get());
//	Dash->PutString("Gear",gear);
}

void DriveTrainClass::Failsafe_Update()
{
	int Ldelta = LeftDriveEnc->Get() - PrevLEnc;
	int Rdelta = RightDriveEnc->Get() - PrevREnc;
	PrevLEnc = LeftDriveEnc->Get();
	PrevREnc = RightDriveEnc->Get();
	if(fabs(LeftDrive->Get()) > .5f)
	{
		if(Ldelta == 0)
		{
			EncoderFailCount++;
		}
	}
	if(fabs(RightDrive->Get()) > .5f)
	{
		if(Rdelta == 0)
		{
			EncoderFailCount++;
		}
	}
	if(EncoderFailCount > 1500)
	{
		TickingEncoders = 0;
	}
	
}
void DriveTrainClass::Failsafe_Reset()
{
	EncoderFailCount = 0;
	TickingEncoders = 1;
	PrevLEnc = LeftDriveEnc->Get();
	PrevREnc = RightDriveEnc->Get();
}
