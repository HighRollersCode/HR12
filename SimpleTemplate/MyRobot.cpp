#include "WPILib.h"
#include "DriveTrain.h"
#include "Intake.h"
#include "Launcher.h"
#include "ShooterWheel.h"
#include "math.h"
#include "TargetingSystemClient.h"
float currentTurretStickX = 0;
float previousTurretStickX = 0;
float turretcommand = 0;
float currentTurretPos = 0;
float targetTurretPos = 0;
int current_ClickIndex;
int previous_ClickIndex;

float CurrentGyroAngle;
float TargetGyroAngle;

int currentholdgyro;
int prevholdgyro;
int ClickIndexNew = 0;

double TargetX;
int ClickNumber;
double AngleX;

class RobotDemo : public SimpleRobot
{
	Joystick *rightStick;			// joystick 2
	Joystick *leftStick;			// joystick 1
	Joystick *turretStick;			// joystick 3
	
	DriverStation *ds;				// driver station object
	
	DriveTrainClass *DriveTrain;
	IntakeClass *Intake;
	LauncherClass *Launcher;
	ShooterWheelClass *ShooterWheel;
		
	Jaguar *Turret;
	//Jaguar *Beater;
//	Compressor *compressor;
		
	Timer *AutonTimer;
	Timer *SendTimer;
	Gyro *TurretGyro;
	
	//Relay *Tomahawk;
	
	TargetingSystemClient *KinectDev;
public:
	RobotDemo(void)
	{
		rightStick = new Joystick(2);			// create the joysticks
		leftStick = new Joystick(1);
		turretStick = new Joystick(3);
		ds = DriverStation::GetInstance();
	
		DriveTrain = new DriveTrainClass();
		Intake = new IntakeClass();
		Launcher = new LauncherClass();
		ShooterWheel = new ShooterWheelClass();
		Turret = new Jaguar(6);
	//	Beater = new Jaguar(6);
	//	compressor = new Compressor(1,1);	//Pressure Switch, Compressor Relay 
		//compressor->Start();
		
		TurretGyro = new Gyro(1,1);
		TurretGyro->SetSensitivity(.007);
		
		AutonTimer = new Timer();

		SendTimer = new Timer();
		SendTimer->Reset();
		SendTimer->Start();
		
		//Tomahawk = new Relay(7);
		KinectDev = new TargetingSystemClient();
		
		KinectDev->Connect("10.9.87.99",9870);
		
	}
	void UpdateDash()
	{
		SmartDashboard *Dash = SmartDashboard::GetInstance();
		if(SendTimer->Get() > .5)
		{
			SendTimer->Reset();
			SendTimer->Start();
			
			static double beat = 0;
			beat += 0.01f;
			Dash->PutDouble("Heartbeat",beat);
						
			DriveTrain->SendData();
			Intake->SendData();
			Launcher->SendData();
			ShooterWheel->SendData();
			 Dash->PutDouble("Gyro",(double)TurretGyro->GetAngle());
			 Dash->PutDouble("TurrCom", (double)turretcommand);
			 Dash->PutDouble("Distance",KinectDev->Get_Target_Distance());
			 Dash->PutBoolean("Kinect",KinectDev->Is_Connected());
			 Dash->PutDouble("Override",ShooterWheel->OverrideCommand);
			 NetworkTable * tbl = NetworkTable::GetTable("987Table");
			if (tbl != NULL)
			{
				ClickNumber = tbl->GetInt("987Seq");
				AngleX = ((float)tbl->GetInt("987Angle"))/1000;
				TargetX = ((float)tbl->GetInt("987ClickX"))/1000;
			}
			else
			{
				TargetX = 7;
				AngleX = 99;
				ClickNumber = 314;
			}
			
			Dash->PutDouble("Click_Pos",TargetX);
			Dash->PutDouble("Angle_Calc",AngleX);
			Dash->PutInt("Click_Index",ClickNumber);
		}
	}
	
	void HoldTurret()
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
	void UpdateTurret()
	{
		//float Targ;
		previousTurretStickX = currentTurretStickX;
		currentTurretStickX = turretStick->GetX();
		
		currentTurretPos = TurretGyro->GetAngle();
		previous_ClickIndex = current_ClickIndex;
		current_ClickIndex = ClickNumber; 
		if(current_ClickIndex > previous_ClickIndex)
		{
			//if(ClickIndexNew > 2)
			//{
				targetTurretPos = (float)(currentTurretPos)+(AngleX);
			//}
			ClickIndexNew++;
		}
							
		if(fabs((double)currentTurretStickX) >= .1f)
		{
			if(fabs((double)previousTurretStickX) <= .1f)
			{
				targetTurretPos = 0;
			}
		}
		if(targetTurretPos == 0)
		{
			turretcommand = -currentTurretStickX;
		}
		else
		{
			float error = targetTurretPos - currentTurretPos;
			
			float multiplier = .15f;
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
	void Wait(float Seconds)
	{
		AutonTimer->Reset();
		AutonTimer->Start();
		while(AutonTimer->Get() < Seconds)
		{
			UpdateDash();
		}
		AutonTimer->Stop();
	}
	void Autonomous(void)
	{
	//	myRobot.SetSafetyEnabled(false);
		DriveTrain->Drive_Auton(.5,.2);
		DriveTrain->ResetEncoders_Timers();
		Launcher->ResetTimers();
		Launcher->ResetTimers();
		ShooterWheel->WheelOff();
		ShooterWheel->SetSpeed(1.0f);
		Wait(4.5);
		Launcher->FullLaunch();
		Wait(.2f);
		Intake->IntakeOn();
		Wait(2.5f);
		Intake->IntakeOff();
		Launcher->FullLaunch();
		Wait(1.5f);
	}
	void OperatorControl(void)
	{		
		GetWatchdog().SetExpiration(.1);//myRobot.SetSafetyEnabled(true);
		TurretGyro->Reset();
		Wait(5.0f);
		ShooterWheel->WheelOff();
		DriveTrain->ResetEncoders_Timers();
		Launcher->ResetTimers();
		
		while (IsOperatorControl())
		{
			GetWatchdog().Feed();
			DriveTrain->Shifter_Update(rightStick->GetTrigger(),rightStick->GetRawButton(11));
		   
			UpdateTurret();
			DriveTrain->Drive_Operator(-leftStick->GetY(),-rightStick->GetY());
			DriveTrain->UpdateEBrake(rightStick->GetRawButton(11));
			Launcher->UpdateLaunch_Operator(turretStick->GetTrigger(),turretStick->GetRawButton(6));
			Intake->UpdateIntake(leftStick->GetRawButton(2),leftStick->GetTrigger());
		//	ShooterWheel->ShooterOverride(turretStick->GetThrottle());
			ShooterWheel->UpdateShooter(turretStick->GetRawButton(3),KinectDev->Get_Target_Distance());
			//HoldTurret();
			UpdateDash();
			
			KinectDev->Set_Turret_Angle(TurretGyro->GetAngle()*3.1415926535f/180.0f);
			KinectDev->Update();
		}
	}
};

START_ROBOT_CLASS(RobotDemo);
