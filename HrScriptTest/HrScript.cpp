#include "HrScript.h"
#include "math.h"
#include "stdio.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <istream>
#include "FakeRobot.h"


///////////////////////////////////////////////////////////////////////////////


HrScriptCommandClass::HrScriptCommandClass()
{
	for (int i = 0; i < MAX_HRSCRIPT_COMMAND_PARAMS; ++i)
	{
		m_Parameters[i] = 0.0f;
	}
}

HrScriptCommandClass::~HrScriptCommandClass()
{
}

void HrScriptCommandClass::Set_Parameter(int i,float val)
{
	if ((i >= 0) && (i < MAX_HRSCRIPT_COMMAND_PARAMS))
	{
		m_Parameters[i] = val;
	}
}

float HrScriptCommandClass::Get_Parameter(int i)
{
	if ((i >= 0) && (i < MAX_HRSCRIPT_COMMAND_PARAMS))
	{
		return m_Parameters[i];
	}
	return 0.0f;
}


///////////////////////////////////////////////////////////////////////////////


HrScriptClass::HrScriptClass()
{
	m_CommandCount = 0;
	for (int i=0; i<MAX_HRSCRIPT_COMMANDS; ++i)
	{
		m_Commands[i] = NULL;
	}
}
	
HrScriptClass::~HrScriptClass()
{
	for (int i=0; i<MAX_HRSCRIPT_COMMANDS; ++i)
	{
		if (m_Commands[i] != NULL)
		{
			delete m_Commands[i];
			m_Commands[i] = NULL;
		}
	}
}

void HrScriptClass::Add_Command(HrScriptCommandClass * cmd)
{
	if (m_CommandCount < MAX_HRSCRIPT_COMMANDS)
	{
		m_Commands[m_CommandCount] = cmd;
		m_CommandCount++;
	}
}

HrScriptCommandClass * HrScriptClass::Get_Command(int i)
{
	if ((i >= 0) && (i < m_CommandCount))
	{
		return m_Commands[i];
	}
	return NULL;
}



///////////////////////////////////////////////////////////////////////////////

HrScriptSystemClass::HrScriptSystemClass()
{
	m_CommandPrototypeCount = 0;
	for (int i=0; i<MAX_COMMAND_PROTOTYPES; ++i)
	{
		m_CommandPrototypes[i] = NULL;
	}

	for (int i=0; i<MAX_AUTO_MODES; ++i)
	{
		m_Scripts[i] = NULL;
	}

}

HrScriptSystemClass::~HrScriptSystemClass()
{
	for (int i=0; i<MAX_COMMAND_PROTOTYPES; ++i)
	{
		if (m_CommandPrototypes[i] != NULL) {
			delete m_CommandPrototypes[i];
			m_CommandPrototypes[i] = NULL;
		}
	}

	for (int i=0; i<MAX_AUTO_MODES; ++i)
	{
		if (m_Scripts[i] != NULL) {
			delete m_Scripts[i];
			m_Scripts[i] = NULL;
		}
	}
}

// Register all of your robots commands
void HrScriptSystemClass::Add_Command(HrScriptCommandClass * command)
{
	if (m_CommandPrototypeCount < MAX_COMMAND_PROTOTYPES)
	{
		m_CommandPrototypes[m_CommandPrototypeCount] = command;
		m_CommandPrototypeCount++;
	}
}
	
// Load and release Scripts
HrScriptClass * HrScriptSystemClass::Load_Script(const char * filename)
{
	printf("\nLoading script %s\n",filename);

	int ignored_lines = 0;
	std::ifstream file(filename);
	if (file.is_open() == false)
	{
		printf("Could not find file: %s",filename);
		return NULL;
	}

	// Create a new script to hold the commands
	HrScriptClass * new_script = new HrScriptClass();
	new_script->Set_Name(filename);	

	while (!file.eof())
	{
		std::string line_string;
		std::string cmd_string;
	
		getline (file,line_string);
		//file.ignore(100,'\n');
		std::istringstream cur_line(line_string);
		
		// first, read the "command" 
		cur_line >> cmd_string;
	
		if ((cmd_string.length() >= 2) && (cmd_string[0] == '/') && (cmd_string[1] == '/'))
		{
			printf("Ignoring comment line: %s\n",line_string.c_str());
			ignored_lines++;
		}
		else
		{
			// start setting up this command
			HrScriptCommandClass * new_cmd = Load_Command(cmd_string);

			// if we got a good command type
			if (new_cmd != NULL)
			{
				// Read params to the end of the line:
				int cur_param = 0;
				float param_value = 0.0f;

				while (!cur_line.eof() && (cur_param < new_cmd->Get_Parameter_Count()))
				{
					cur_line >> param_value;
					new_cmd->Set_Parameter(cur_param,param_value);
					cur_param++;
				}
			
				// add the command to the array	
				new_script->Add_Command(new_cmd);

				// Debug display all params
				printf("Found Command: %s ",new_cmd->Get_Command_Name());
				for (int j=0; j<new_cmd->Get_Parameter_Count(); ++j)
				{
					printf(" %f ",new_cmd->Get_Parameter(j));
				}
				printf("\n");
			}
			else
			{
				if (cmd_string.length() > 0)
				{
					printf(" Unrecognized Command: %s\n",cmd_string.c_str());
				}
				ignored_lines++;
			}
		}
	}
	printf("Loaded script %s, %d commands, %d ignored lines\n",filename,new_script->Get_Command_Count(), ignored_lines);

	return new_script;
}

HrScriptCommandClass * HrScriptSystemClass::Load_Command(std::string cmd_string)
{
	// Simple loop over the prototypes to see if any match the command string that was given:
	for (int i=0; i<m_CommandPrototypeCount; ++i)
	{
		if (cmd_string == m_CommandPrototypes[i]->Get_Command_Name())
		{
			return m_CommandPrototypes[i]->Create_Command();
		}
	}

	return NULL;
}

void HrScriptSystemClass::Release_Script(HrScriptClass *script)
{
	if (script != NULL)
	{
		delete script;
	}
}

// Run a script
void HrScriptSystemClass::Execute_Script(HrScriptClass * script)
{
	if (script == NULL) return;
	
	printf("\nExecuting Script: %s\n",script->Get_Name());
	
	int count = script->Get_Command_Count();
	for (int i=0; i<count; ++i)
	{
		script->Get_Command(i)->Execute();
	}
}

// Autonomous Scripts
void HrScriptSystemClass::Set_Auto_Script(int index,const char * script_filename)
{
	if ((index >= 0) && (index < MAX_AUTO_MODES))
	{	
		if (m_Scripts[index] != NULL)
		{
			delete m_Scripts[index];
			m_Scripts[index] = NULL;
		}
		HrScriptClass * new_script = Load_Script(script_filename);
		m_Scripts[index] = new_script;
	}
}

void HrScriptSystemClass::Run_Auto_Script(int index)
{
	if ((index >= 0) && (index < MAX_AUTO_MODES))
	{
		Execute_Script(m_Scripts[index]);
	}
}

void HrScriptSystemClass::Load_And_Run_Script(const char * filename)
{
	HrScriptClass * script = Load_Script(filename);
	Execute_Script(script);
	Release_Script(script);
}
