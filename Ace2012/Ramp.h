#ifndef RAMP_H
#define RAMP_H
#include "WPIlib.h"

#define ramp_Up 0
#define ramp_Down 1
#define ramp_Raising 2
#define ramp_Lowering 3


class RampClass
{
	Relay *Tomahawk;
	Servo *Lock;
	DigitalInput *Limit;
	Timer *State_Timer;
	float current_timer;
	float previous_timer;
	int LimitState;
	int CurrentToggleBtn;
	int PrevToggleBtn;
	int state;
	
private:
	
	void Update_Raise();
	void Update_Lower();
	
public:
	RampClass();
	~RampClass();
	void SetState(int s);
	void Update_Ramp(int ToggleBTNState, int raise, int lower,float testserv);
	void SendData();
};
#endif

