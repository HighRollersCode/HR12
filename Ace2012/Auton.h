#ifndef AUTON_H
#define AUTON_H

#include "WPILib.h"
#include "DriveTrain.h"
#include "Intake.h"
#include "Launcher.h"
#include "ShooterWheel.h"
#include "Ramp.h"
#include "Turret.h"
#include "Watchdog.h"
#include "TargetingSystemClient.h"

class AutonomousController
{
	
private:
	Watchdog *watchdog;
	DriveTrainClass *DriveTrain;
	IntakeClass *Intake;
	LauncherClass *Launcher;
	ShooterWheelClass *ShooterWheel;
	RampClass *RampDeploy;
	TurretClass *Turret;
	DriverStation *ds;
	TargetingSystemClient *KinectDev;

	
	Timer *AutonTimer;
	Timer *SendTimer;
	
	KinectStick *lstick;
	KinectStick *rstick;
	
	bool Abort;
	float leftstickkinect;
	bool Running();
	void AutonWait(float Seconds);
	void AutonWait2(float Seconds, int brake);
	bool Auto_System_Update();
	void Auto_FullLaunch()
	{
		if(Running())Launcher->FullLaunch();
	}
	void Auto_IntakeOn()
	{
		if(Running())Intake->IntakeOn();
	}
	void Auto_IntakeOff()
	{
		if(Running())Intake->IntakeOff();
	}
	
	void Auto_DriveTimer(float Forward, float Turn, float Seconds);
	void Auto_DriveEncoder(float Forward, float Turn, float Ticks);
	void Auto_GYROTURN(float heading);
	void Auto_GYROSTRAIGHT(float forward,float ticks, float desheading);
	void Auto_DriveGyro_Encoder(float Forward, float Angle, float Ticks);
	void Auto_LowerTomahawk();
	void Auto_RaiseTomahawk();
	void Auto_Kinect_Aim(float sec);
	void Auto_Kinect_Aim2(float delay_between_corr);
	//void Au
	void SendData();
	void AUTO_Wait_Kinect(int n);
	
public:
	AutonomousController
	(
		DriveTrainClass *D,
		IntakeClass *I,
		LauncherClass *L,
		ShooterWheelClass *S,
		RampClass *R,
		TurretClass *T,
		Watchdog *W,
		DriverStation *Ds,
		TargetingSystemClient *kd
	);
	~AutonomousController(){}
	void Auto_Start();
	void Auto_End();
	void AUTO_eating();
	void AUTO_Bridge_Score();
	void AUTO_Test_Kinect_Aiming();
	void AUTO_Layup();
	void AUTO_KINECTSPEED();
	void AUTO_2ball();
	void AUTO_BridgeSteal();
	void AUTO_AllianceBridge();
	void FireAll();

};
#endif

