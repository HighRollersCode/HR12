#include "WPIlib.h"

class ShooterWheelClass
{	
	int currentShooterBTN;
	int previousShooterBTN;
	int ShooterToggle; //1
	Jaguar *Shooter;
	Jaguar *Shooter2;
	GearTooth *GearSensor;
	//float dist;
		
public:
	float OverrideCommand;
	ShooterWheelClass();
	~ShooterWheelClass();
	
	void SetSpeed(float command);
	void WheelOff();
	void UpdateShooter(int Enable,float distance);
	void ShooterOverride(float input);
	float CalcCommand(float dist);
	void SendData();
};
