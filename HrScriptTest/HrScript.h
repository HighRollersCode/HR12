#pragma once

#include "stdio.h"
#include <string>


/*

	HrScript - High Rollers Script Language for FRC robots

	Design Goals (in order of importance):
	- Ability to modify and test autonomous mode routines on our robot without the compile-download-reboot time.
	- Ability to define robot settings and update them without compile-download-reboot time.
	- Simplest, smallest amount of code possible, easily understood by students.
	- Support Multiple autonomous modes.

	Ideas:
	- The 'language' is essentially 1 command per line with 'n' floating point parameters.
	- Blank lines, comment lines, and comments at the end of lines are ignored
	- Add new commands to the system by deriving from a base class and registering a command object.
	

	Robot Organization Idea:

	- There could be one master file which has the robot settings (PID constants, speeds, etc)
	- This file could have a list of what script to run for each autonomous mode.

	>>>>> RobotSettings.hrs >>>>>>
	// Robot Constants
	SetArmP 10.0
	SetArmI  0.0
	SetArmD  1.0

	SetDriveTurnScale 3.7

	// Autonomous Scripts
	SetAutoScript 1	LeftSideScoreThenAllianceBridge.hrs		<---- Maybe leave this setup in C++ to simplify the script parsing.
	SetAutoScript 2 RightSideScore.hrs
	SetAutoScript 3 EatAndScore.hrs



	>>>> EatAndScore.hrs >>>>>>>>

	Intake 1.0
	WaitForBall
	Shoot
	Intake 1.0
	WaitForBall
	Shoot



	- Example Robot Initialization:

	void Init()
	{
		....

		// Register all Commands
		TheScriptSystem.Add_Command(new SetArmPCommand());
		TheScriptSystem.Add_Command(new SetArmICommand());
		TheScriptSystem.Add_Command(new IntakeCommand());
		TheScriptSystem.Add_Command(new ShootCommand());

		Load_Scripts()

	}

	void Load_Scripts()	// Run this at init *OR* when we want to reload
	{

		HrScriptClass * robot_settings_script = TheScriptSystem.Load_Script("RobotSettings.hrs");
		TheScriptSystem.Execute_Script(robot_settings_script);
		TheScriptSystem.Release_Script(robot_settings_script);

		// Decide which scripts to run on the auto indexes here in C++
		TheScriptSystem.Set_Auto_Script(0,TheScriptSystem.Load_Script("EatAndScore.hrs");
		TheScriptSystem.Set_Auto_Script(1,TheScriptSystem.Load_Script("BridgeScore.hrs");
		TheScriptSystem.Set_Auto_Script(2,TheScriptSystem.Load_Script("RightScore.hrs");

		TheScriptSystem.Set_Auto_Script(15,TheScriptSystem.Load_Script("TestAuto.hrs");


		....
	}


	-- Example Command, derives from HrScriptCommand

	class DriveTicksHeadingCommand : public HrScriptCommand
	{
	public:
		virtual const char * Get_Command_Name() { "DriveTicksHeading" }		
		virtual int Get_Parameter_Count() { return 2; }
		virtual HrScriptCommandClass * Create_Command() { return new DriveTicksHeadingCommand(); }

		virtual void Execute()
		{
			Robot->Reset_Encoders();
			int ticks = 0;
			while ((Command_Should_Exit() == false) && (ticks < m_Parameters[0]))
			{
				Update_Robot_Subsystems();
				
				// Proprotional steering based on heading, etc
			}
		}
	};

	class SetArmPIDCommand : public HrScriptCommand
	{
	public:
		virtual const char * Get_Command_Name() { "SetArmPID" }		
		virtual int Get_Parameter_Count() { return 3; }
		virtual HrScriptCommandClass * Create_Command() { return new SetArmPIDCommand(); }

		virtual void Execute()
		{
			Robot->Set_Arm_Constants(m_Parameter[0],m_Parameter[1],m_Parameter[2]);
		}
	};



*/

	
const int MAX_HRSCRIPT_COMMAND_PARAMS = 16;
const int MAX_HRSCRIPT_COMMANDS = 256;
const int MAX_AUTO_MODES = 16;
const int MAX_COMMAND_PROTOTYPES = 256;


class HrScriptCommandClass
{
public: 
	HrScriptCommandClass();
	virtual ~HrScriptCommandClass();

	// User implements these so the system can create your command as it parses a script
	virtual const char * Get_Command_Name() = 0;		
	virtual int Get_Parameter_Count() = 0;
	virtual HrScriptCommandClass * Create_Command() = 0;

	// Implement the Execute function which will run a script executes your command
	virtual void Execute() = 0;

	// access to the parameters
	void Set_Parameter(int i,float val);
	float Get_Parameter(int i);

protected:

	float m_Parameters[MAX_HRSCRIPT_COMMAND_PARAMS];
};




class HrScriptClass
{
public:
	HrScriptClass(void);
	~HrScriptClass(void);

	void Add_Command(HrScriptCommandClass * cmd);
	int Get_Command_Count() { return m_CommandCount; }
	HrScriptCommandClass * Get_Command(int i);

	void Set_Name(const char * name) { m_Name = name; }
	const char * Get_Name(void) { return m_Name.c_str(); }

protected:

	std::string m_Name;
	int m_CommandCount;
	HrScriptCommandClass * m_Commands[MAX_HRSCRIPT_COMMANDS];
};


class HrScriptSystemClass
{
public:
	HrScriptSystemClass();
	~HrScriptSystemClass();

	// Register all of your robots commands
	void Add_Command(HrScriptCommandClass * command);
	
	// Load and release Scripts
	HrScriptClass * Load_Script(const char * filename);
	void Release_Script(HrScriptClass *);

	// Run a script
	void Execute_Script(HrScriptClass * script);

	// Autonomous Scripts
	void Set_Auto_Script(int index,const char * script_filename);
	void Run_Auto_Script(int index);

	// Helper function, loads, runs, releases a script
	void Load_And_Run_Script(const char * filename);

protected:

	HrScriptCommandClass * Load_Command(std::string cmd_string);

	// One of each command the user's robot has registered.
	int m_CommandPrototypeCount;
	HrScriptCommandClass * m_CommandPrototypes[MAX_COMMAND_PROTOTYPES];

	// Various Autonomous mode scripts
	HrScriptClass * m_Scripts[MAX_AUTO_MODES];

};

