#include "FakeRobot.h"



FakeRobot * FakeRobot::TheRobot = NULL;	

FakeRobot::FakeRobot(void)
{
	TheRobot = this;
	Init_Script_System();
}


FakeRobot::~FakeRobot(void)
{
	TheRobot = NULL;
}

void FakeRobot::Auto()
{
	m_ScriptSystem->Run_Auto_Script(1);
}

void FakeRobot::OperatorControl()
{
	Load_Scripts();
	m_ScriptSystem->Run_Auto_Script(0);
}




bool FakeRobot::Script_Should_Exit()
{
	return false;
}

void FakeRobot::Update_Robot_Subsystems()
{
}

void FakeRobot::Drive_Ticks_Heading_Command(float ticks, float heading)
{
	printf("Running Command Drive_Ticks_Heading %f %f\n",ticks,heading);
}

void FakeRobot::Set_Arm_PID(float p,float i, float d)
{
	printf("Running Command Set_Arm_PID %f %f %f\n",p,i,d);
}

