#include "ShooterWheel.h"
///// DIST:  5.7,    10.7
///// BANKS: 1501.0, 1730.0
///// SwISH: 1400,   1614

#define Shooter_Preset_Low 1538.0f
#define Shooter_Preset_Med 1690.0f
#define Shooter_Preset_High 3000.0f

static float rpEr;
static float rpDes;
static float cmdCorrection;
//
// RPM -> Motor Command Table
//
static float RPMTable[] = { 1032.0, 1190.0, 1365.0, 1516.0, 1675.0, 1810.0,  1958.0 };
static float MotorCmd[] = { 0.4,    0.45,   0.5,    0.55,   0.6,    0.648,   0.7   };

//static float NEWDIST[] = {5.5f, 10};
//static float NEWRPMBANK[] = {1538.0f, 1756.3f};

static float NEWDIST[] = {8.8f, 10.75f};
static float NEWRPMBANK[] = {1538.0, 1690.0f};
//static float NEWDIST[] = {7.2f, 10.0f};
//static float NEWRPMBANK[] = {1538.0, 1690.0f};
//static float NEWRPMSWISH[] = {1400.0f,1636.0f};//x, .681
static int RPM_TABLE_COUNT = sizeof(NEWRPMBANK) / sizeof(NEWRPMBANK[0]);

static double currenttime = 0.0;
static double previoustime = 0.0;

ShooterWheelClass::ShooterWheelClass()
{
	Shooter = new Jaguar(5);
	Shooter2 = new Jaguar(9);
	GearSensor = new GearTooth(1,10,false);
	GearSensor->Reset();
	GearSensor->Start();
	
	ShooterToggle = 1; 
	State = ShooterState_off;
	
	currentPresetSpeed = 0.0f;
	OverrideCommand = 0.0f;

	RPMCOUNT = 0.0;
	PREVRPMCOUNT = 0.0;
	RPM = 0.0;
	DTIME = 0.0;
	DCOUNT = 0.0;
	INDICATOR = 0;
}

void ShooterWheelClass::SetSpeed(float command)
{
	Shooter->Set(command);
}
float ShooterWheelClass::USELINE(float dist)
{
	if(dist > 15.0f)
	{
		dist = 15.0f;
	}
	float normdist = (float)((dist - NEWDIST[0])/(NEWDIST[1]-NEWDIST[0]));
	if(normdist < 0){normdist = 0;}
	float RPMCALC =NEWRPMBANK[0]+(normdist*(NEWRPMBANK[1]-NEWRPMBANK[0]));
	return RPMCALC;
}

float ShooterWheelClass::EstimatePower(float desiredRPM)
{
	if(desiredRPM < RPMTable[0]){return MotorCmd[0];}
	if(desiredRPM > RPMTable[RPM_TABLE_COUNT-1]){return MotorCmd[RPM_TABLE_COUNT-1];}
	
	for(int i = 0; i < RPM_TABLE_COUNT; i++)
	{
		if(desiredRPM < RPMTable[i])
		{
			float norm = (desiredRPM - RPMTable[i-1])/(RPMTable[i] - RPMTable[i-1]);
			float command = (MotorCmd[i-1] + norm*(MotorCmd[i]-MotorCmd[i-1]));
			return command;
		}
	}
	return MotorCmd[RPM_TABLE_COUNT-1];
}
float ShooterWheelClass::PUpdate(float desrpm)
{
	
	double basecom = EstimatePower(desrpm);
	double error = desrpm - RPM;
	double mult = .01;
	double correction = (error*mult);
	if (correction > 0.5) 
	{
		correction = 0.5;
	}
	if (correction < -0.2f)//.2 
	{
		correction = -0.2f;
	}
	double command = correction + basecom;
	if (command > 1.0)
	{
		command = 1.0;
	}
	
	cmdCorrection = correction;
	rpDes = desrpm;
	rpEr = error;
	return command;
	
}
void ShooterWheelClass::ShooterOverride(float input)
{
	
	float normval = (input+1.0f)/2.0f;
	
	float MIN_COM = .4f;
	float MAX_COM = 1.0f;
	
	//float MIN_COM = 1400.0f;
	//float MAX_COM = 1900.0f;
	OverrideCommand = MIN_COM+normval*(MAX_COM-MIN_COM);
	
}
void ShooterWheelClass::ShooterOverrideRPM(float rpm)
{
	OverrideCommand = rpm;
}
void ShooterWheelClass::WheelOff()
{
	Shooter->Set(0.0f);
	ShooterToggle = 1;
}
void ShooterWheelClass::UpdateShooter(
		int EnableLow,
		int EnableMed,
		int EnableHigh,
		int EnableKinect,
		int EnableOverride,
		float distance,
		double RobotTime)//turretstickrawbutton3
{
	RPMCOUNT = GearSensor->Get();
	currenttime = RobotTime;
	double dt = currenttime-previoustime;

	if((dt > 0.01f) || (dt < 0.0f))
	{
		DTIME = dt;
		DCOUNT = (RPMCOUNT-PREVRPMCOUNT);
		//RPM = ((DCOUNT)/(DTIME));
		//RPM = (RPM/1.0)*60.0;
		RPM = ((1.0/(GearSensor->GetPeriod()))*60.0)/6;
		previoustime = currenttime;
		PREVRPMCOUNT = RPMCOUNT;
		//printf("RPM calc. dt: %f  dcount: %f  rpm:  %f  time:   %f\r\n",(float)DTIME,(float)(DCOUNT),(float)RPM,(float)RobotTime);
	}
	if((EnableLow == 1)&&(prevlow == 0))
	{
		SetState(ShooterState_low);
		currentPresetSpeed = Shooter_Preset_Low;
	}
	if((EnableMed == 1)&&(prevmed == 0))
	{
		SetState(ShooterState_med);
		currentPresetSpeed = Shooter_Preset_Med;
	}
	if((EnableHigh == 1)&&(prevhigh == 0))
	{
		SetState(ShooterState_high);
		currentPresetSpeed = Shooter_Preset_High;
	}
	if((EnableKinect == 1)&&(prevkinect == 0))
	{
		SetState(ShooterState_kinect);
	}
	if((EnableOverride == 1)&&(prevoverride == 0))
	{
		SetState(ShooterState_override);
	}
	
	if(ShooterToggle == 1)
	{
		SetSpeed(0.0f);//-.78
		INDICATOR = 0;
	}
	else if(ShooterToggle == -1)
	{
		INDICATOR = 1;
		if(State == ShooterState_kinect)
		{
		    currentPresetSpeed = USELINE(distance); //CalcCommandDist_RPM(dist);
		    
		}
		else if(State == ShooterState_overrideRPM)
		{
			currentPresetSpeed = (OverrideCommand);
		}
		else
		{
			
		}
		//SetSpeed(.64f);//55
		//SetSpeed(CalcCommand(distance));
		if(State == ShooterState_override)
		{
			SetSpeed(OverrideCommand); // currentPresetSpeed);
		}
		else
		{
			SetSpeed(PUpdate(currentPresetSpeed));
		}
		
		//SetSpeed(currentPresetSpeed);
		
		//SetSpeed(PUpdate(5.0f));
	}
	else
	{
		SetSpeed(0.0f);
	}
	Shooter2->Set(Shooter->Get());
	prevlow = EnableLow;
	prevhigh = EnableHigh;
	prevmed = EnableMed;
	prevkinect = EnableKinect;
	prevoverride = EnableOverride;
}
void ShooterWheelClass::SetState(int newstate)
{
	ShooterToggle = -ShooterToggle;
	State = newstate;
}

void ShooterWheelClass::SendData()
{
	SmartDashboard *Dash = SmartDashboard::GetInstance();
	Dash->PutDouble("Shooter Command",Shooter->Get());
	Dash->PutInt("G_T_Reading", GearSensor->Get());
	Dash->PutDouble("RPM",RPM); //(1.0/GearSensor->GetPeriod())*60.0);	//RPM
	Dash->PutDouble("DesiredRPM",rpDes);
	Dash->PutDouble("ErrorRPM",rpEr);
	//Dash->PutDouble("RPMCorr",cmdCorrection);
}
