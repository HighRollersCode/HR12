#include "WPILib.h"
#include "DriveTrain.h"
#include "Intake.h"
#include "Launcher.h"
#include "ShooterWheel.h"
#include "Ramp.h"
#include "Turret.h"
#include "Auton.h"
#include "Grabber.h"
#include "math.h"
#include "TargetingSystemClient.h"
#include "KinectStick.h"
// make a new header called "AceDefines.h"

int SendDataCurr = 0;
int SendDataPrev = 0;
int SendDataToggle = 1;

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
	RampClass *RampDeploy;
	TurretClass *Turret;
	TargetingSystemClient *KinectDev;
	AutonomousController *AutonomousContol;
	GrabberClass *Grabber;
	
	Timer *GameTimer;
	Timer *SendTimer;
	
	//Command *AutoChoice;
	//SendableChooser *AutoChooser;
	Compressor *compressor;
#if !PRACTICE_BOT
	DigitalInput *DIP_1;
	DigitalInput *DIP_2;
	DigitalInput *DIP_3;
	DigitalInput *DIP_4;
#endif
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
		RampDeploy = new RampClass();
		Turret = new TurretClass();
		Grabber = new GrabberClass();
		// Disable the watchdog while we connect to the Kinect
		GetWatchdog().SetEnabled(false);
		KinectDev = new TargetingSystemClient();
		KinectDev->Connect("10.9.87.99",9870);
		Turret->Reset_Sensors();
		//GetWatchdog().SetEnabled(true);
		//4692
		GetWatchdog().Feed();
		AutonomousContol = new AutonomousController
		(
			DriveTrain,
			Intake,
			Launcher,
			ShooterWheel,
			RampDeploy,
			Turret,
			&GetWatchdog(),
			ds,
			KinectDev
		);
		compressor = new Compressor(14,1);	//Pressure Switch, Compressor Relay 
		compressor->Start();
#if !PRACTICE_BOT
		DIP_1 = new DigitalInput(5);
		DIP_2 = new DigitalInput(6);
		DIP_3 = new DigitalInput(7);
		DIP_4 = new DigitalInput(9);
#endif
		GetWatchdog().Feed();
		GameTimer = new Timer();
		GameTimer->Reset();
		GameTimer->Start();
		SendTimer = new Timer();
		SendTimer->Reset();
		SendTimer->Start();
		GetWatchdog().Feed();
		
//		AutoChooser = new SendableChooser();
//		AutoChooser->AddDefault("4-Ball",(Command)(AutonomousContol->AUTO_Bridge_Score()));
	}
	//-.8+-.1;
	void UpdateDash()
	{
		SmartDashboard *Dash = SmartDashboard::GetInstance();
		if(SendTimer->Get() > .05)
		{
			if(SendDataToggle == -1)
			{
				SendTimer->Reset();
				SendTimer->Start();
				static double beat = 0;
				beat += 0.01f;
				Dash->PutDouble("Heartbeat",beat);
							
				DriveTrain->SendData();
				Turret->SendData();
				Intake->SendData();
			
				ShooterWheel->SendData();
				RampDeploy->SendData();
				
				Dash->PutDouble("Angle(Kct)",KinectDev->Get_Target_Angle());
				Dash->PutDouble("Bank",KinectDev->Get_Target_Bank());
				Dash->PutBoolean("Kinect",KinectDev->Is_Connected());
				Dash->PutDouble("Override",ShooterWheel->OverrideCommand);
				Dash->PutDouble("Distance",KinectDev->Get_Target_Distance());
			}
			Dash->PutInt("^",ShooterWheel->INDICATOR);
		
			Launcher->SendData();
		}
		Turret->GetData();
	}	
	void Autonomous(void)
	{
		GetWatchdog().SetEnabled(false);
		AutonomousContol->Auto_Start();
#if !PRACTICE_BOT
		if(DIP_1->Get() == 1)
		{
			AutonomousContol->AUTO_2ball();
		}
		else if(DIP_2->Get() == 1)
		{
			AutonomousContol->AUTO_Layup();
		}
		else if(DIP_3->Get() == 1)
		{
			AutonomousContol->AUTO_eating();
		}
		else if(DIP_4->Get() == 1)
		{
			AutonomousContol->AUTO_BridgeSteal();
		}
#else
		AutonomousContol->AUTO_Layup();
#endif
		
		
	//	AutonomousContol->AUTO_KINECTSPEED();
		AutonomousContol->Auto_End();
	}
	void OperatorControl(void)
	{		
		GetWatchdog().SetExpiration(.1);
		GetWatchdog().SetEnabled(true);
		GetWatchdog().Feed();

		ShooterWheel->WheelOff();
		DriveTrain->ResetEncoders_Timers();
		Launcher->ResetTimers();
		Grabber->ResetTimers();
		while (IsOperatorControl())
		{
			GetWatchdog().Feed();
			DriveTrain->Shifter_Update(rightStick->GetTrigger(),rightStick->GetRawButton(11));
		   
			Turret->UpdateTurret(turretStick->GetX(),turretStick->GetRawButton(7),KinectDev->Get_Target_Angle(),KinectDev->Get_Target_Bank());
			DriveTrain->Drive_Operator(-leftStick->GetY(),-rightStick->GetY());
			DriveTrain->UpdateEBrake(rightStick->GetRawButton(10),100);
			Launcher->UpdateLaunch_Operator(turretStick->GetTrigger(),turretStick->GetRawButton(6));
			Intake->UpdateIntake(leftStick->GetRawButton(2),leftStick->GetTrigger());
			ShooterWheel->ShooterOverride(turretStick->GetThrottle());
			ShooterWheel->UpdateShooter(turretStick->GetRawButton(4),
					turretStick->GetRawButton(3),
					turretStick->GetRawButton(5),
					turretStick->GetRawButton(2),
					turretStick->GetRawButton(10),
					KinectDev->Get_Target_Distance(),
					GameTimer->Get());
			Grabber->UpdateGrabber(rightStick->GetRawButton(3));
			UpdateDash();
			float g = (float)((leftStick->GetThrottle()+1)/2);
			RampDeploy->Update_Ramp(rightStick->GetRawButton(2),leftStick->GetRawButton(4),leftStick->GetRawButton(5),g);
		//	KinectDev->Set_Turret_Angle(Turret->TurretGyro->GetAngle()*3.1415926535f/180.0f);
			KinectDev->Update();
			SendDataPrev = SendDataCurr;
			SendDataCurr = turretStick->GetRawButton(9);
			if((SendDataCurr == 1)&&(SendDataPrev == 0))
			{
				SendDataToggle = -SendDataToggle;
			}
			Wait(.001f);
		}
	}
};

START_ROBOT_CLASS(RobotDemo);
