#include "WPILib.h"
#include "HRScript.h"



class RobotDemo : public SimpleRobot
{
	Joystick *rightStick;			// joystick 2
	Joystick *leftStick;			// joystick 1
	Joystick *turretStick;			// joystick 3
	
	DriverStation *ds;				// driver station object
	AutonomousControllerClass *AutonomousContol;
    
	Compressor *compressor;
public:
	RobotDemo(void)
	{
		rightStick = new Joystick(2);			// create the joysticks
		leftStick = new Joystick(1);
		turretStick = new Joystick(3);
		ds = DriverStation::GetInstance();
		SmartDashboard *Dash = SmartDashboard::GetInstance();
		AutonomousContol = new AutonomousControllerClass(ds);
	
		compressor = new Compressor(14,1);	//Pressure Switch, Compressor Relay 
		compressor->Start();
		Dash->PutInt("Initialized", 1);
	}
	void UpdateDash()
	{
		SmartDashboard *Dash = SmartDashboard::GetInstance();
		Dash->PutInt("dashworks",207);
		AutonomousContol->SendData();
	}	
	void Autonomous(void)
	{	
		SmartDashboard *Dash = SmartDashboard::GetInstance();
		AutonomousContol->Parse("Auto_Test1.txt");
		Dash->PutInt("Parsed",1);
		
		AutonomousContol->Execute();	
		Dash->PutInt("InAuto", 1);
		
		GetWatchdog().SetEnabled(false);
	}

	void OperatorControl(void)
	{		
		GetWatchdog().SetExpiration(.1);
		GetWatchdog().SetEnabled(true);
		GetWatchdog().Feed();

		while (IsOperatorControl())
		{
			GetWatchdog().Feed();	   				
			UpdateDash();
			Wait(.001f);
		}
	}
};

START_ROBOT_CLASS(RobotDemo);

