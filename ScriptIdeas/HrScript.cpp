#include "HRScript.h"
#include "math.h"

#include "stdio.h"

int data = -1;
int data2 = -1;
int badcoms = 0;

AutonomousControllerClass::AutonomousControllerClass( DriverStation *Ds)
{
	ds = Ds;
}
void AutonomousControllerClass::Parse(const char * filename)
{
	m_Commands.clear();
	badcoms = 0;
	data = -1;
 	data2 = -1;
	ifstream script(filename);
	std::string cmd_string;
	std::string line_string;
		
	while (!script.eof())
	{
		getline (script,line_string);
		script.ignore(100,'\n');
		std::istringstream cur_line(line_string);
		
		// first, read the "command" 
		cur_line >> cmd_string;
	
		// start setting up this command
		ScriptCommandClass cmd;
		cmd.m_CmdType = CMD_INVALID;		
		
		// set up the command
		if (cmd_string == "WAIT") cmd.m_CmdType = CMD_WAIT;
		else if (cmd_string == "MOVE_GYRO") cmd.m_CmdType = CMD_MOVE_GYRO_ENCODER;
		else if (cmd_string == "TURN") cmd.m_CmdType = CMD_TURN_GYRO;
		
		// if we got a good command type
		if (cmd.m_CmdType != CMD_INVALID)
		{
			// Read params to the end of the line:
			int cur_param = 0;
			while (!cur_line.eof() && (cur_param < MAX_PARAMS))
			{
				cur_line >> cmd.m_Params[cur_param];
				cur_param++;
			}
			// add the command to the array	
			m_Commands.push_back(cmd);
		}
		else{badcoms++;}
	}
	data2 = m_Commands.size();
}
void AutonomousControllerClass::Execute_Command(ScriptCommandClass & cmd)
	{
		// first, ALWAYS just exit if auto is over, etc...
		if (!Running()) return;
		
		switch(cmd.m_CmdType) 
		{
		case CMD_WAIT: 
			Execute_Wait(cmd.m_Params[0]);
			break;
		case CMD_MOVE_GYRO_ENCODER: 
			Execute_Move_Gyro_Encoder(cmd.m_Params[0],cmd.m_Params[1],cmd.m_Params[2]);
			break;
		case CMD_TURN_GYRO: 
			Execute_Turn_Gryo(cmd.m_Params[0]);
			break;
		default:
			break;
		}
	}
void AutonomousControllerClass::Execute()
{
	for (unsigned int i=0; i<m_Commands.size(); ++i)
	{
		Execute_Command(m_Commands[i]); 
	}
}
void AutonomousControllerClass::Auto_System_Update()
{

}
bool AutonomousControllerClass::Running()
	{
		//if(Abort){return false;}
		// if (AutonTimer->Get() >= 15.0f){return false;}
		//else if (DriveTrain->TickingEncoders == 0){return false;}
	  if (ds->IsAutonomous() == false){return false;}
		else if (ds->IsEnabled() == false){return false;}
		//else if ((fabs(lstick->GetY() +.8f) < .1f) && (fabs(rstick->GetY() + .8f) < .1f)){return false;}
		return true;
	}
void AutonomousControllerClass::Execute_Wait(float Seconds)
{
	/*
	float targ = AutonTimer->Get() + Seconds;
	while((AutonTimer->Get() < targ)&&(Running()))
	{
		Auto_System_Update();
	}
	*/
	data = 1;
	//data2 = Seconds;
}

void AutonomousControllerClass::Execute_Move_Gyro_Encoder(float forward,float ticks,float desheading)
{
	data = 2;
	/*
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
	 */
	 
}
void  AutonomousControllerClass::Execute_Turn_Gryo(float heading)
{
	data = 3;
	//data2 = heading;
}
void  AutonomousControllerClass::SendData()
{
	SmartDashboard *Dash = SmartDashboard::GetInstance();
	Dash->PutInt("CommandIndex",data);
	Dash->PutInt("Param",data2);
	Dash->PutInt("Invalids",badcoms);
}
