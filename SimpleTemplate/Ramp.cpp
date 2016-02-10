#include "Ramp.h"


RampClass::RampClass()
{

	Tomahawk = new Relay(8,Relay::kBothDirections);
	Lock = new Servo(1);
	Limit = new DigitalInput(8);
	
	LimitState = 0;
	CurrentToggleBtn = 0;
	PrevToggleBtn = 0;
}
/*
void IntakeClass::IntakeOn()
{
	Uptake->Set(1.0f);
	Uptake_->Set(Relay::kForward);
	//Beater->Set(1.0f);
}
void IntakeClass:: IntakeOff()
{
	Uptake->Set(0.0f);
	Uptake_->Set(Relay::kOff);
	//Beater->Set(0.0f);
}
void IntakeClass::UpdateIntake(int OUTBTN, bool INTRIG)//leftstickgetbtn2, leftstickgetgrig
{
	if((OUTBTN == 0) && (INTRIG == false))
	{
		Uptake->Set(0);
		Uptake_->Set(Relay::kOff);
		//Beater->Set(0);
		IsIntakeOn = 0;
		IsOuttakeOn = 0;
		
		
		UptakeStatus = "Off";
	}
	else if((OUTBTN == 1) && (INTRIG == true))
	{
		Uptake->Set(0);
		Uptake_->Set(Relay::kOff);
		//Beater->Set(0);
		IsIntakeOn = 0;
		IsOuttakeOn = 0;
		UptakeStatus = "Off";
	}
	else if ((OUTBTN == 0) && (INTRIG == true))
	{
		Uptake->Set(1.0f);
		Uptake_->Set(Relay::kForward);
		//Beater->Set(1.0f);
		IsIntakeOn = 1;
		IsOuttakeOn = 0;
		UptakeStatus = "Intake";
	}
	else if ((OUTBTN == 1) && (INTRIG == false))
	{
		Uptake->Set(-1.0f);
		Uptake_->Set(Relay::kReverse);
		//Beater->Set(-1.0f);
		IsIntakeOn = 0;
		IsOuttakeOn = 1;
		UptakeStatus = "Outtake";
	}
	
}
void IntakeClass::SendData()
{
	SmartDashboard *Dash = SmartDashboard::GetInstance();
	Dash->PutString("Uptake",UptakeStatus);

}

*/
