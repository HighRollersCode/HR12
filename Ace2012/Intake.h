#ifndef INTAKE_H
#define INTAKE_H

#include "WPIlib.h"

class IntakeClass
{
	Jaguar *Uptake;
	string UptakeStatus;
	Relay *Uptake_;
	
public:
	
	int IsIntakeOn;
	int IsOuttakeOn;
	
	IntakeClass();
	~IntakeClass();
	
	void IntakeOn();
	void IntakeOff();
	void UpdateIntake(int OUTBTN, bool INTRIG);
	void SendData();
};
#endif
