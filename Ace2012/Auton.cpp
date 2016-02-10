#include "Auton.h"
#include "math.h"
#include "KinectStick.h"
#include "stdio.h"

//#define AUTO_LOG x if(log != NULL) fprintf x
//#define AUTO_LOG x 

//FILE *logauto = NULL;

float stick1 = 0.0f;
float stick2 = 0.0f;
int autoindex = 0;
AutonomousController::AutonomousController
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
)
{
	DriveTrain = D;
	Intake = I;
	Launcher = L;
	ShooterWheel = S;
	RampDeploy = R;
	Turret = T;
	watchdog = W;
	ds = Ds;
	KinectDev = kd;
	AutonTimer = new Timer();
	SendTimer = new Timer();
	Abort = false;
	leftstickkinect = 0.0f;
	lstick = new KinectStick(1);
	rstick = new KinectStick(2);

}
void AutonomousController::Auto_Start()
{
//	log = fopen("autolog.txt","wa");
	watchdog->SetExpiration(.1);
	watchdog->SetEnabled(true);
	watchdog->Feed();
	AutonTimer->Reset();
	AutonTimer->Start();
	Launcher->ResetTimers();
	SendTimer->Reset();
	SendTimer->Start();
	Turret->targetTurretPos = 0.0f;
	Turret->TurretGyro->Reset();
	DriveTrain->Failsafe_Reset();
	Auto_System_Update();
}
void AutonomousController::Auto_End()
{
	ShooterWheel->WheelOff();
}
bool AutonomousController::Running()
{
	if(Abort){return false;}
	else if (AutonTimer->Get() >= 15.0f){return false;}
	//else if (DriveTrain->TickingEncoders == 0){return false;}
	else if (ds->IsAutonomous() == false){return false;}
	else if (ds->IsEnabled() == false){return false;}
	else if ((fabs(lstick->GetY() +.8f) < .1f) && (fabs(rstick->GetY() + .8f) < .1f)){return false;}
	return true;
}
void AutonomousController::AutonWait(float Seconds)
{
	float targ = AutonTimer->Get() + Seconds;
	while((AutonTimer->Get() < targ)&&(Running()))
	{
		Auto_System_Update();
	}
}
void AutonomousController::AutonWait2(float Seconds,int brake)
{
	float targ = AutonTimer->Get() + Seconds;
	while((AutonTimer->Get() < targ)&&(Running()))
	{
		Auto_System_Update();
		DriveTrain->UpdateEBrake(1,brake);
	}
	DriveTrain->Drive_Auton(0.0f,0.0f);
}
void AutonomousController::Auto_DriveTimer(float Forward, float Turn, float Seconds)
{
	DriveTrain->Drive_Auton(Forward,Turn);
	AutonWait(Seconds);
	DriveTrain->Drive_Auton(0,0);
}
void AutonomousController::Auto_DriveEncoder(float Forward, float Turn, float Ticks)
{
	DriveTrain->ResetEncoders_Timers();
	if(Ticks > 0)
	{
		while((DriveTrain->GetLeftEncoder() < Ticks)&&(Running()))
		{
			DriveTrain->Drive_Auton(Forward, Turn);
			Auto_System_Update();
			DriveTrain->Failsafe_Update();
		}
	}
	else
	{
		while((DriveTrain->GetLeftEncoder() > Ticks)&&(Running()))
		{
			DriveTrain->Drive_Auton(Forward, Turn);
			Auto_System_Update();
			DriveTrain->Failsafe_Update();
		}
	}
	DriveTrain->Drive_Auton(0,0);
//	DriveTrain->Failsafe_Reset();
}
void AutonomousController::Auto_GYROTURN(float heading)
{
	Turret->targetTurretPos = 0;
	DriveTrain->ResetEncoders_Timers();
	
	float GYRO_P = -0.02f;
	float angle_error = heading - Turret->currentTurretPos;
	float turn = GYRO_P * angle_error;
	while((fabs(angle_error) > 3.0f)&&(Running()))
	{
		angle_error = heading - Turret->currentTurretPos;
		turn = -(GYRO_P * angle_error);
		Auto_System_Update();
		DriveTrain->Drive_Auton(0,turn);
	}
}
void AutonomousController::Auto_GYROSTRAIGHT(float forward,float ticks,float desheading)
{
	DriveTrain->ResetEncoders_Timers();
	float MAINTAIN = desheading;
	float GYRO_P = -0.04f;
	if(forward < .6)
	{
		GYRO_P = -0.04f;
	}
	else
	{
		GYRO_P = -0.01f;
	}
	float angle_error = MAINTAIN - Turret->currentTurretPos;
	float turn = GYRO_P * angle_error;
	
	if(ticks > 0)
	{
		while((DriveTrain->GetLeftEncoder() < ticks)&&(Running()))
		{
			angle_error = MAINTAIN - Turret->currentTurretPos;
			turn = -(GYRO_P * angle_error);
			DriveTrain->Drive_Auton(forward, turn);
			Auto_System_Update();
			DriveTrain->Failsafe_Update();
		}
	}
	else
	{
		while((DriveTrain->GetLeftEncoder() > ticks)&&(Running()))
		{
			angle_error = MAINTAIN - Turret->currentTurretPos;
			turn = -(GYRO_P * angle_error);
			DriveTrain->Drive_Auton(forward, turn);
			Auto_System_Update();
			DriveTrain->Failsafe_Update();
		}
	}
	DriveTrain->Drive_Auton(0,0);
	//DriveTrain->Failsafe_Reset();
}
void AutonomousController::Auto_DriveGyro_Encoder(float Forward, float Angle, float Ticks)
{
	DriveTrain->ResetEncoders_Timers();
	Auto_GYROTURN(Angle);
	AutonWait(.7f);
	Auto_GYROSTRAIGHT(Forward,Ticks,Angle);
}
void AutonomousController::Auto_LowerTomahawk()
{
	if(Running()){RampDeploy->SetState(ramp_Lowering);}
}
void AutonomousController::Auto_RaiseTomahawk()
{
	if(Running()){RampDeploy->SetState(ramp_Raising);}
}
void AutonomousController::Auto_Kinect_Aim(float sec)
{
	float angle = KinectDev->Get_Target_Angle();
	Turret->SetTarg(angle);
	AutonWait(sec);
} 

void AutonomousController::Auto_Kinect_Aim2(float delay_between_corr)
{
	float angle = KinectDev->Get_Target_Angle();
	Turret->SetTarg(angle);
	while (Running())
	{
		float angle = KinectDev->Get_Target_Angle();
		Turret->SetTarg(angle);
		return;
		if (fabs(angle) < 1.50f)
		{
			return;
		}
		
		AutonWait(delay_between_corr);
	}
}
void AutonomousController::AUTO_Wait_Kinect(int n)
{
	int counter = 0;
	
	while((counter < n)&&(Running()))
	{
		
		if(Auto_System_Update() == true)
		{
			counter++;
		}
		
	}
	
}
bool AutonomousController::Auto_System_Update()
{	
	watchdog->Feed();
	if(Running())
	{
		Launcher->UpdateBlock_AUTO();
		ShooterWheel->UpdateShooter(0,0,0,0,0,KinectDev->Get_Target_Distance(),AutonTimer->Get());
		Turret->UpdateTurret(0.0f,0,0.0f,0.0f);
		DriveTrain->Shifter_Update(0,0);
	}
	SendData();
	
	RampDeploy->Update_Ramp(0,0,0,0.0f);
	 return KinectDev->Update();
}
void AutonomousController::SendData()
{
	SmartDashboard *Dash = SmartDashboard::GetInstance();

	if(SendTimer->Get() > .05)
	{
		SendTimer->Reset();
		SendTimer->Start();
		//static double beat = 0;
		//beat += 0.01f;
	//	Dash->PutDouble("Heartbeat",beat);
					
		//DriveTrain->SendData();
		//Turret->SendData();
		//Intake->SendData();
		//Launcher->SendData();
		//ShooterWheel->SendData();
		//RampDeploy->SendData();
	//	Dash->PutDouble("Distance",KinectDev->Get_Target_Distance());
		//Dash->PutDouble("autoindex",autoindex);
		//Dash->PutBoolean("Kinect",KinectDev->Is_Connected());
		//Dash->PutDouble("Override",ShooterWheel->OverrideCommand);
		//Dash->PutDouble("Angle(Kct)",KinectDev->Get_Target_Angle());
		//Dash->PutDouble("lefthand",leftstickkinect);
	}
}
void AutonomousController::AUTO_eating()
{
	ShooterWheel->SetState(ShooterState_overrideRPM);
	ShooterWheel->OverrideCommand = 1690.0f;
	AutonWait(1.0f);//2.2f
	Auto_IntakeOn();

	while((AutonTimer->Get() < 15.0f)&&(Running()))
	{
		while((Launcher->HoldingBall == false)&&(Running()))
		{
			Auto_System_Update();
		}
		AutonWait(2.0f);
		Auto_FullLaunch();
		//AutonWait(1.0f); //.5 was perfect // .7
	}
}

//1500 low
//290 high
void AutonomousController::AUTO_Bridge_Score()
{
	ShooterWheel->SetState(ShooterState_overrideRPM);
	ShooterWheel->ShooterOverrideRPM(1690);// 1790.0f;
	Auto_LowerTomahawk();
	// launch one ball
	AutonWait(2.50f);
	Auto_FullLaunch();
	Auto_IntakeOn();
	
	
	// wait for next ball
	DriveTrain->ToggleState = 1;// -1 for low
	while((Launcher->HoldingBall == false)&&(Running()))
	{
		Auto_System_Update();
	}
	//1733 rpm far shot
	//1210 `lowgear encoder left far shot
	// settle, then launch second
	AutonWait(1.8f);//2.65f
	Auto_FullLaunch();

	Auto_IntakeOn();
	//far shot:
	//Auto_DriveEncoder(1.0f,0,1210);
	/* close shot =*/
	Auto_DriveEncoder(.6f,0,585);//1500 old encoders // 3700 low gear new encoders
	AutonWait(2.5f);
	Auto_IntakeOff();
	ShooterWheel->OverrideCommand = 1538.0f;
	ShooterWheel->SetState(ShooterState_kinect);
	ShooterWheel->ShooterToggle = -1;
	Auto_RaiseTomahawk();
//	DriveTrain->ToggleState = 1;
	Auto_DriveEncoder(-1.0f,.0f,-450);//.08f//-200 for real bot use ticks*2 // use .07 turn if needed
	//float angle = KinectDev->Get_Target_Angle();
	AutonWait(.5f);
	Auto_IntakeOn();
	
	Auto_Kinect_Aim2(.5f);
	
	AutonWait(1.1f);
	Auto_FullLaunch();
	AutonWait(.2f);
	Auto_IntakeOn();

	while((Launcher->HoldingBall == false)&&(Running()))
	{
		Auto_System_Update();
	}
	AutonWait(2.0f);	
	Auto_FullLaunch();
	AutonWait(4.0f);
	ShooterWheel->OverrideCommand = 0.0f;
}

void AutonomousController::AUTO_Layup()// 4ball alliance bridge
{
//	AUTO_LOG((log,"startlayup\r\n"));
	ShooterWheel->SetState(ShooterState_overrideRPM);
	ShooterWheel->ShooterOverrideRPM(1538);// 1670.0f;
	DriveTrain->ToggleState = 1;
	ShooterWheel->ShooterToggle = -1;
	AutonWait(.25f);
//	AUTO_LOG((log,"test = %d\r\n",ShooterWheel->ShooterToggle));
	Auto_GYROSTRAIGHT(-1.0f,-220,0.0f);
	ShooterWheel->ShooterToggle = -1;
	//Auto_GYROSTRAIGHT(-.1f,-200,0.0f);
	//Auto_DriveTimer(.1f,0.0f,.025f);
	AutonWait2(.2f,-10);
	ShooterWheel->ShooterToggle = -1;
	DriveTrain->UpdateEBrake(0,0);
	Turret->SetTarg(-35.0f);
	AutonWait(1.0f);
	Auto_FullLaunch();
	Auto_IntakeOn();

	while((Launcher->HoldingBall == false)&&(Running()))
	{
		Auto_System_Update();
	}
	AutonWait(1.3f);
	Auto_FullLaunch();
	AutonWait(.2f);
	ShooterWheel->WheelOff();
	
	Turret->targetTurretPos = 0.0f;
	Turret->Reset_Sensors();
	autoindex = 1;
	Auto_LowerTomahawk();
	Auto_GYROSTRAIGHT(.35f,50,-40.0f);
	Auto_GYROSTRAIGHT(1.0f,250,-40.0f);
	Auto_GYROSTRAIGHT(.35f,230,-40.0f);

	Auto_GYROSTRAIGHT(.35f,50,0.0f);
	Auto_GYROSTRAIGHT(.8f,100,0.0f);
	Auto_GYROSTRAIGHT(.2f,540,0.0f);
	autoindex = 2;
	Auto_IntakeOn();
#if !PRACTICE_BOT
	while((Launcher->HoldingBall == false)&&(Running()))
	{
		Auto_System_Update();
		DriveTrain->UpdateEBrake(1,150);
	}
#else
	AutonWait2(2.0f,150);
#endif
	autoindex = 3;
	DriveTrain->UpdateEBrake(0,0);
	
	Auto_GYROSTRAIGHT(-.4f,-20,0.0f);
	Auto_GYROSTRAIGHT(-3.5f,-50,-30.0f);

	ShooterWheel->SetState(ShooterState_kinect);
	ShooterWheel->ShooterToggle = -1;
	Auto_GYROSTRAIGHT(-1.0f,-750,-36.0f);//	Auto_GYROSTRAIGHT(-1.0f,-850,-30.0f);
	autoindex = 4;
	AutonWait2(.1f,-10);
	Turret->SetTarg(50.0f);//Turret->SetTarg(37.0f);
	AutonWait(1.0f);
	autoindex = 5;
	AUTO_Wait_Kinect(4);
	autoindex = 6;
	Auto_Kinect_Aim2(0.4f);
	AutonWait(.2f);
	//autoindex = 7;
	//Turret->SetTarg(.5f);

	//AutonWait(.f);
	Auto_FullLaunch();
	FireAll();

}
void AutonomousController::AUTO_Test_Kinect_Aiming()
{
	while (Running())
	{
		Auto_Kinect_Aim2(1.0f);
	
		AutonWait(1.5f);
		Auto_DriveTimer(0.0f,0.5f,0.5f);
		
		AutonWait(0.5f);

		Auto_Kinect_Aim2(1.0f);

		AutonWait(1.5f);
		Auto_DriveTimer(0.0f,-0.5f,0.5f);

		AutonWait(0.5f);
	}
}
void AutonomousController::AUTO_KINECTSPEED()
{
	ShooterWheel->SetState(ShooterState_override);
	
	while(Running())
	{
		leftstickkinect = lstick->GetY();
		Auto_System_Update();
		ShooterWheel->ShooterOverride(leftstickkinect);
	}
}
void AutonomousController::AUTO_2ball()
{
	//Line up with intake on key line. Shoots preloads and knocks bridge
	ShooterWheel->SetState(ShooterState_overrideRPM);
	ShooterWheel->ShooterOverrideRPM(1690);
	Auto_LowerTomahawk();
	AutonWait(1.5f);
	Auto_FullLaunch();
	Auto_IntakeOn();
	DriveTrain->ToggleState = 1;
	AutonWait(.5f);
	while((Launcher->HoldingBall == false)&&(Running()))
	{
		Auto_System_Update();
	}
	AutonWait(1.5f);//2.65f
	Auto_FullLaunch();
	Auto_IntakeOn();
	AutonWait(.25f);
	Auto_FullLaunch();
	Auto_GYROSTRAIGHT(1.0f,100,0.0f);
	Auto_GYROSTRAIGHT(.2f,500,0.0f);
	Auto_IntakeOn();
		while((Launcher->HoldingBall == false)&&(Running()))
		{
			Auto_System_Update();
			DriveTrain->UpdateEBrake(1,150);
		}
		DriveTrain->UpdateEBrake(0,0);
	Auto_IntakeOff();
	ShooterWheel->OverrideCommand = 1538.0f;
	//ShooterWheel->SetState(ShooterState_kinect);
	ShooterWheel->ShooterToggle = -1;
	Auto_IntakeOn();
	Auto_GYROSTRAIGHT(-1.0f,-600,0.0f);
	Auto_RaiseTomahawk();

	AutonWait(.5f);
	Auto_IntakeOn();
	
	Auto_Kinect_Aim2(.2f);
	Turret->SetTarg(1.0f);
	AutonWait(1.1f);
	Auto_FullLaunch();
	AutonWait(.35f);
	Auto_IntakeOn();
	while((Launcher->HoldingBall == false)&&(Running()))
	{
		Auto_System_Update();
	}
	
	FireAll();
	ShooterWheel->OverrideCommand = 0.0f;
}

void AutonomousController::AUTO_BridgeSteal()
{
	Auto_LowerTomahawk();
	AutonWait(1.0f);

	Auto_GYROSTRAIGHT(1.0f,100,0.0f);
	Auto_GYROSTRAIGHT(.4f,500,0.0f);//640

	AutonWait2(4.0f,150);

	
	Auto_IntakeOff();
	ShooterWheel->OverrideCommand = 1538.0f;
	ShooterWheel->SetState(ShooterState_kinect);
	ShooterWheel->ShooterToggle = -1;
	
	Auto_GYROSTRAIGHT(-1.0f,-750,0.0f);
	AutonWait2(.3f,0);
	Auto_RaiseTomahawk();
	//AutonWait(.5f);
	Auto_Kinect_Aim2(.5f);
	
	AutonWait(1.1f);
	
	Auto_FullLaunch();
	FireAll();

}
void AutonomousController::AUTO_AllianceBridge()
{
	AutonWait(.3f);
	Turret->SetTarg(68.50f);
	ShooterWheel->SetState(ShooterState_overrideRPM);
	ShooterWheel->ShooterOverrideRPM(1800);
	
	AutonWait(2.5f);

	Auto_FullLaunch();
	Auto_IntakeOn();
	DriveTrain->ToggleState = 1;
	while((Launcher->HoldingBall == false)&&(Running()))
	{
		Auto_System_Update();
	}
	AutonWait(1.5f);//2.65f
	Auto_FullLaunch();
	//FireAll();
	Turret->targetTurretPos = 0.0f;
	AutonWait(1.5f);
	ShooterWheel->ShooterToggle = 1;
	Turret->Reset_Sensors();
	Auto_GYROSTRAIGHT(.3f,318,0.0f);
	Auto_GYROTURN(70.0f);
	Auto_LowerTomahawk();
	AutonWait(2.0f);
	Auto_GYROSTRAIGHT(.4f,660,90.0f);
	while(Launcher->HoldingBall == false)
	{
		Auto_IntakeOn();
		Auto_System_Update();
		DriveTrain->Drive_Auton(.2f, 0.0f );
	}
	AutonWait(.5f);
	Auto_IntakeOff();
	Auto_GYROSTRAIGHT(-.7f,-800,70.0f);
	Auto_GYROSTRAIGHT(-.7f,-200,90.0f);
	Turret->SetTarg(-160.0f);
	AutonWait(2.5f);
	Auto_Kinect_Aim2(0.7f);
	Turret->SetTarg(2.0f);

	ShooterWheel->SetState(ShooterState_kinect);
	ShooterWheel->ShooterToggle = -1;
	FireAll();
}
void AutonomousController::FireAll()
{
	Auto_IntakeOn();
	while(Running())
	{
		while((Launcher->HoldingBall == false)&&(Running()))
		{
			Auto_System_Update();
		}
		AutonWait(1.5f);
		Auto_FullLaunch();
	}
}
