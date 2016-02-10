#include "FakeRobot.h"
#include "HrScript.h"


// Create commands for the robot scripts


class DriveTicksHeadingCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "DriveTicksHeading"; }		
	virtual int Get_Parameter_Count() { return 2; }
	virtual HrScriptCommandClass * Create_Command() { return new DriveTicksHeadingCommand(); }

	virtual void Execute()
	{
		FakeRobot::Get()->Drive_Ticks_Heading_Command(m_Parameters[0], m_Parameters[1]);
	}
};

class SetArmPIDCommand : public HrScriptCommandClass
{
public:
	virtual const char * Get_Command_Name() { return "SetArmPID"; }		
	virtual int Get_Parameter_Count() { return 3; }
	virtual HrScriptCommandClass * Create_Command() { return new SetArmPIDCommand(); }

	virtual void Execute()
	{
		FakeRobot::Get()->Set_Arm_PID(m_Parameters[0],m_Parameters[1],m_Parameters[2]);
	}
};







void FakeRobot::Init_Script_System()
{
	m_ScriptSystem = new HrScriptSystemClass();

	// register commands with the system
	m_ScriptSystem->Add_Command(new DriveTicksHeadingCommand());
	m_ScriptSystem->Add_Command(new SetArmPIDCommand());

	Load_Scripts();
}

void FakeRobot::Load_Scripts()
{
	// Load and run the robot settings script:
	m_ScriptSystem->Load_And_Run_Script("RobotSettings.hrs");

	// Load all of the auto-mode scripts
	m_ScriptSystem->Set_Auto_Script(0,"AutoBridgeScore.hrs");
	m_ScriptSystem->Set_Auto_Script(1,"AutoEat.hrs");

}
