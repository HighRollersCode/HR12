#include "WPIlib.h"

class GrabberClass
{
	int currToggleState;
	int prevToggleState;
	
	Solenoid *GrabberOut;
	Solenoid *GrabberIn;
	
	Timer *Grabber_Delay;
	
	int toggle;
	void Engage();
	//void DisEngage();
public:
	
	GrabberClass();
	~GrabberClass();
		
	void UpdateGrabber(int togglebuttonstate);
	void ResetTimers();
	void SendData();
};

