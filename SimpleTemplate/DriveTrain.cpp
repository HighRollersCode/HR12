
#include "DriveTrain.h"

DriveTrainClass::DriveTrainClass()
{

	LeftSideTicks = 0;
	RightSideTicks = 0;

	current_AutonTicks = 0;
	
	RightDrive = new Jaguar(1);//1
	RightDrive_a = new Jaguar(2);//2
	LeftDrive = new Jaguar(3); //3
	LeftDrive_a = new Jaguar(4);//4
	LeftDriveEnc = new Encoder(3,4);
	RightDriveEnc = new Encoder(1,2);
	
	ShifterHigh = new Solenoid(1,1);
	ShifterLow = new Solenoid(1,2);
	NeutralHigh = new Solenoid(1,7);
	NeutralLow = new Solenoid(1,8);
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

void DriveTrainClass::UpdateEBrake(int enable)
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
		RightSideTicks = RightDriveEnc->Get();
	
		float Lerror = LeftSideTicks;
		float Rerror = RightSideTicks;
	
		float Leftout = Lerror*.55f;
		float Rightout = Rerror*.55f;
		
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
	LeftDrive->Set(Forward+Turn);
	RightDrive->Set(Forward+Turn);
	
	LeftDrive_a->Set(Forward+Turn);
	RightDrive_a->Set(Forward+Turn);

}
void DriveTrainClass::Drive_AutonEncoder(float Forward, float Turn, int Ticks)
{
	current_AutonTicks = 0;
	if(Ticks > current_AutonTicks)
	{
		while(current_AutonTicks < Ticks)
		{
			Drive_Auton(Forward, Turn);
		}
	}
}
void DriveTrainClass::Drive_AutonTimer(float Forward, float Turn, float Seconds)
{
	Drive_Auton(Forward,Turn);
	Wait(Seconds);
	Drive_Auton(0,0);
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
		Neutral_DelayTM->Reset();
		Neutral_DelayTM->Start();
		if(neutral == false)
		{
			ToggleState = -ToggleState;
		}
		else
		{
			neutral = false;
			ToggleState = 1;
		}
	}
	if(PrevNeutralBut == false && NeutralBut == true)
	{
		neutral = true;
		Neutral_DelayTM->Reset();
		Neutral_DelayTM->Start();
	}
	if(neutral == true)
	{
		ShifterHigh->Set(false);
		ShifterLow->Set(true);
		if(Neutral_DelayTM->Get() > .250)
		{
			NeutralHigh->Set(false);
			NeutralLow->Set(true);		
		}
		
	}
	else if(ToggleState == 1)
	{
		NeutralHigh->Set(true);
		NeutralLow->Set(false);
		if(Neutral_DelayTM->Get() > .250)
		{
			ShifterHigh->Set(true);
			ShifterLow->Set(false);
		}
	}
	else if(ToggleState == -1)
	{
		NeutralHigh->Set(true);
		NeutralLow->Set(false);
		if(Neutral_DelayTM->Get() > .250)
		{
			ShifterHigh->Set(false);
			ShifterLow->Set(true);
		}
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
    Dash->PutInt("Right Drive Encocder",RightDriveEnc->Get());
	Dash->PutDouble("Left Drive Command",(double)LeftDrive_a->Get());
	Dash->PutDouble("Right Drive Command",(double)RightDrive_a->Get());
	Dash->PutString("Gear",gear);
}



