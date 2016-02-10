#ifndef HRSCRIPT_H
#define HRSCRIPT_H

#include "stdio.h"
#include "WPILib.h"
#include "yvals.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <istream>
//
// High Rollers Script
//

// SIMPLEST way to represent a bunch of commands
// One structure, with a "type" ID and some max number of parameters.
// every "command" is one of these structures.  So the structure holds the settings for a command

// Representation of the Parsed Script (i.e. list of commands and params)
enum CommandType 
{
	CMD_INVALID = -1,
	
	CMD_WAIT = 0,
	CMD_MOVE_GYRO_ENCODER,
	CMD_TURN_GYRO,
	CMD_ENABLE_SHOOTER,
};

const int MAX_PARAMS = 4;

class ScriptCommandClass
{
public:

	CommandType 	m_CmdType;
	float			m_Params[MAX_PARAMS];
	
};

class AutonomousControllerClass
{
	DriverStation *ds;
private:
	
public:
	AutonomousControllerClass(DriverStation *Ds);
		
	~AutonomousControllerClass(){}
	void Auto_System_Update();
	bool Running();
	
	void Execute_Command(ScriptCommandClass & cmd);

	void Execute_Wait(float Seconds);
	void Execute_Move_Gyro_Encoder(float forward,float ticks,float desheading);
	void Execute_Turn_Gryo(float heading);
	void SendData();
		
	std::vector<ScriptCommandClass>  m_Commands;
	void Parse(const char * filename);

	// run the script
	void Execute();
};
#endif


