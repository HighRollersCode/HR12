#include "Turret.h"
#include "math.h"

TurretClass::TurretClass()
{
	currentTurretStickX = 0;
	previousTurretStickX = 0;
	turretcommand = 0;
	currentTurretPos = 0;
	targetTurretPos = 0;
	current_ClickIndex = 0;
	previous_ClickIndex = 0;
	
	currentholdgyro = 0;
	prevholdgyro = 0;
	ClickIndexNew = 0;

	TargetX = 0;
	ClickNumber = 0;
	AngleX = 0;
	CalcOffset = 0.0f;
	Turret = new Jaguar(6);
	TurretGyro = new Gyro(1,1);
	TurretGyro->SetSensitivity(.007);
	KinectAim = 0;
}
TurretClass::~TurretClass(){}
/*
void TurretClass::HoldTurret()
	{
		prevholdgyro = currentholdgyro;
		currentholdgyro = turretStick->GetRawButton(11);
		if((currentholdgyro == 1)&&(prevholdgyro == 0))
		{
			TargetGyroAngle = CurrentGyroAngle;
		}
		if(currentholdgyro == 1)
		{
			CurrentGyroAngle = TurretGyro->GetAngle();
			float error = CurrentGyroAngle - TargetGyroAngle;
			float Mult = .7f;
			float motorOut = error*Mult;
			Turret->Set(motorOut);
		}
	}
	*/
void TurretClass::SetTarg(float v)
{
	targetTurretPos = v + TurretGyro->GetAngle();
}
void TurretClass::UpdateTurret(float Input,int kinecttoggle, float kinectangle,float kinectbankangle)
{
	previousTurretStickX = currentTurretStickX;
	currentTurretStickX = Input;
	float k = .1f;
	CalcOffset = k*kinectbankangle;
	previous_ClickIndex = current_ClickIndex;
	current_ClickIndex = ClickNumber; 
	KinectAim = kinecttoggle;
	if(KinectAim == 1)
	{
		targetTurretPos = 0.0f;
		currentTurretPos = -kinectangle;
	}
	else
	{
		currentTurretPos = TurretGyro->GetAngle();
		if(current_ClickIndex > previous_ClickIndex)
		{
			//if(ClickIndexNew > 2)
			//{
				targetTurretPos = (float)(currentTurretPos)+(AngleX);
			//}
			ClickIndexNew++;
		}
	}
	
				
	if(fabs((double)currentTurretStickX) >= .1f)
	{
		if(fabs((double)previousTurretStickX) <= .1f)
		{
			targetTurretPos = 0;
		}
	}
	if((targetTurretPos == 0)&&(KinectAim == 0))
	{
		
		turretcommand = -currentTurretStickX;
	}
	else
	{
		float error = targetTurretPos - currentTurretPos;
		float multiplier;
		if(KinectAim == 1)
		{
			multiplier = .05f;
		}
		else
		{
			multiplier = .15f;
		}
		float min = 0.0f;
		if(error > 0)
		{
			min = .01f;
		}
		else
		{
			min = -.01f;
		}
		
		float motorout = -(error*multiplier) + min;
		turretcommand = motorout;
	}
	Turret->Set(turretcommand);
}

void TurretClass::Reset_Sensors()
{
	TurretGyro->Reset();
}
void TurretClass::SendData()
{
	SmartDashboard *Dash = SmartDashboard::GetInstance();
	Dash->PutDouble("Gyro",(double)TurretGyro->GetAngle());
	Dash->PutDouble("TurrCom", (double)turretcommand);
	Dash->PutDouble("TurrTarget",(double)targetTurretPos);
	
	//	Dash->PutDouble("Click_Pos",TargetX);
	//	Dash->PutDouble("Angle_Calc",AngleX);
	//	Dash->PutInt("Click_Index",ClickNumber);
}
void TurretClass::GetData()
{
	NetworkTable * tbl = NetworkTable::GetTable("987Table");
	if (tbl != NULL)
	{
		//tbl->BeginTransaction();
		tbl->GetInt("987Seq");
		((float)tbl->GetInt("987Angle"))/1000;
		((float)tbl->GetInt("987ClickX"))/1000;
	//	ClickNumber = tbl->GetInt("987Seq");
	//	AngleX = ((float)tbl->GetInt("987Angle"))/1000;
	//	TargetX = ((float)tbl->GetInt("987ClickX"))/1000;
		//tbl->EndTransaction();
	}
	else
	{
	//	TargetX = 7;
	//	AngleX = 99;
	//	ClickNumber = 314;
	}
}

