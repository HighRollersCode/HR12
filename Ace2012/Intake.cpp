#include "Intake.h"

IntakeClass::IntakeClass()
{
	Uptake = new Jaguar(7); 
	IsIntakeOn = 0;
	IsOuttakeOn = 0;
	UptakeStatus = "";
	Uptake_ = new Relay(7);
}
void IntakeClass::IntakeOn()
{
	Uptake->Set(1.0f);
	Uptake_->Set(Relay::kForward);
}
void IntakeClass:: IntakeOff()
{
	Uptake->Set(0.0f);
	Uptake_->Set(Relay::kOff);
}
void IntakeClass::UpdateIntake(int OUTBTN, bool INTRIG)//leftstickgetbtn2, leftstickgetgrig
{
	if((OUTBTN == 0) && (INTRIG == false))
	{
		Uptake->Set(0);
		Uptake_->Set(Relay::kOff);
		IsIntakeOn = 0;
		IsOuttakeOn = 0;
		UptakeStatus = "Off";
	}
	else if((OUTBTN == 1) && (INTRIG == true))
	{
		Uptake->Set(0);
		Uptake_->Set(Relay::kOff);
		IsIntakeOn = 0;
		IsOuttakeOn = 0;
		UptakeStatus = "Off";
	}
	else if ((OUTBTN == 0) && (INTRIG == true))
	{
		Uptake->Set(1.0f);
		Uptake_->Set(Relay::kForward);
		IsIntakeOn = 1;
		IsOuttakeOn = 0;
		UptakeStatus = "Intake";
	}
	else if ((OUTBTN == 1) && (INTRIG == false))
	{
		Uptake->Set(-1.0f);
		Uptake_->Set(Relay::kReverse);
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

