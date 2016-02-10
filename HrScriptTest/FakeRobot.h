#pragma once

#include "HrScript.h"


class FakeRobot
{
public:
	FakeRobot(void);
	~FakeRobot(void);

	static FakeRobot * Get() { return TheRobot; }

	void Auto();
	void OperatorControl();


	
	// Autonomous Functions
	bool Script_Should_Exit();
	void Update_Robot_Subsystems();

	void Drive_Ticks_Heading_Command(float ticks, float heading);
	void Set_Arm_PID(float p,float i, float d);

protected:
	void Init_Script_System();
	void Load_Scripts();

	static FakeRobot * TheRobot;	
	HrScriptSystemClass * m_ScriptSystem;

};

