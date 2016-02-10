#ifndef SHOOTER_H
#define SHOOTER_H

#include "WPIlib.h"
enum ShooterState
{
	ShooterState_off = 0,
	ShooterState_low,
	ShooterState_med,
	ShooterState_high,
	ShooterState_kinect,
	ShooterState_override,
	ShooterState_overrideRPM
};

class ShooterWheelClass
{	
	
	int prevlow;
	int prevmed;
	int prevhigh;
	int prevkinect;
	int prevoverride;
	int State;
	float currentPresetSpeed;
	
	Jaguar *Shooter;
	Jaguar *Shooter2;
	GearTooth *GearSensor;
	double DTIME;
	double DCOUNT;
	//float dist;
	
public:
	int INDICATOR;
	double RPMCOUNT;
	double RPM;
	double PREVRPMCOUNT;
	float OverrideCommand;
	int ShooterToggle; //1
	ShooterWheelClass();
	~ShooterWheelClass();
	
	void SetSpeed(float command);
	void WheelOff();
	void UpdateShooter(int EnableLow,int EnableMed,int EnableHigh,
			int EnableKinect,int EnableOverride,float distance,double RobotTime);
	void ShooterOverride(float input);
	void ShooterOverrideRPM(float rpm);
	float EstimatePower(float desiredRPM);
	
	float PUpdate(float dist);
	float USELINE(float dist);
	void SetState(int newstate);
	void SendData();
};
#endif
