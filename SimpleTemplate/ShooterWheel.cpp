#include "ShooterWheel.h"

/* 

Shooter Wheel Calibration:

	Distance:
	
	5.0			.56speed @100%layup
	7.0         .59speed
	






*/

//banks:.55,.59,.625,.654
//swish: .543,?,.59,.628

float dists[5] = {5.4f,7.0f,8.8f,11.0f,13.0f};
float commands[5] = { 0.55f, 0.59f, 0.59f, 0.628f, 0.66f};
float commands_Swish_Only[5] = { 0.543f, .55f, 0.59f, 0.628f, 0.66f};
float commands_Bank_Only[5] = { 0.55f, 0.59f, 0.625f, 0.654f, 0.66f};
int TABLECOUNT = sizeof(commands)/sizeof(commands[0]);


ShooterWheelClass::ShooterWheelClass()
{
	currentShooterBTN = 0;
	previousShooterBTN = 0;
	ShooterToggle = 1; 
	Shooter = new Jaguar(5);
	GearSensor = new GearTooth(1,10,true);
	GearSensor->Reset();
	GearSensor->Start();
	OverrideCommand = 0.0f;
	//dist = 0.0f;
}

void ShooterWheelClass::SetSpeed(float command)
{
	Shooter->Set(command);
}
float ShooterWheelClass::CalcCommand(float dist)
{
	if(dist < dists[0]){return commands[0];}
	//if(dist >= dists[3]){return commands[3];}
	for(int i = 1; i < TABLECOUNT;i++)
	{
		if(dist < dists[i])
		{
			float normdist = (dist - dists[i-1])/(dists[i] - dists[i-1]);
			float command = (commands[i-1] + normdist*(commands[i]-commands[i-1]));
			return command;
		}
	}
	return commands[TABLECOUNT-1];
}

void ShooterWheelClass::ShooterOverride(float input)
{
	float normval = (input+1.0f)/2.0f;
	float MIN_COM = .4f;
	float MAX_COM = .7f;
	OverrideCommand = MIN_COM+normval*(MAX_COM-MIN_COM);
	
}
void ShooterWheelClass::WheelOff()
{
	Shooter->Set(0.0f);

	//ShooterToggle = 1;
}
void ShooterWheelClass::UpdateShooter(int Enable, float distance)//turretstickrawbutton3
{
	previousShooterBTN = currentShooterBTN;
	currentShooterBTN = Enable;
	
	if((currentShooterBTN == 1)&&(previousShooterBTN == 0 ))
	{
		ShooterToggle = -ShooterToggle;
	}
	if(ShooterToggle == 1)
	{
		SetSpeed(0.0f);//-.78
	}
	else if(ShooterToggle == -1)
	{
		//SetSpeed(.64f);//55
		SetSpeed(CalcCommand(distance));
	}
	else
	{SetSpeed(0.0f);}
}
void ShooterWheelClass::SendData()
{
	SmartDashboard *Dash = SmartDashboard::GetInstance();
	Dash->PutDouble("Shooter Command",Shooter->Get());
	Dash->PutInt("G_T_Reading", GearSensor->Get());

}
