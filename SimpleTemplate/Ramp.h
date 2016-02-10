#include "WPIlib.h"

class RampClass
{
	Relay *Tomahawk;
	Servo *Lock;
	DigitalInput *Limit;
	
	int LimitState;
	int CurrentToggleBtn;
	int PrevToggleBtn;
	
public:
	
		
	RampClass();
	~RampClass();
	
	void FullDeploy();
	void FullRetract();
	void Update_Ramp(int ToggleBTNState);
	void SendData();
};
