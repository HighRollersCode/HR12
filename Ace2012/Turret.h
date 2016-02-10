#ifndef TURRET_H
#define TURRET_H
#include "WPIlib.h"

class TurretClass
{
	
private:
	float currentTurretStickX;
	float previousTurretStickX;
	float turretcommand;
	
	
	int current_ClickIndex;
	int previous_ClickIndex;

	int currentholdgyro;
	int prevholdgyro;
	int ClickIndexNew;
	
	double TargetX;
	int ClickNumber;
	double AngleX;
	
	Jaguar *Turret;
	int KinectAim;
	


	
public:
	float CalcOffset;
	float currentTurretPos;
	float targetTurretPos;
	Gyro *TurretGyro;
	TurretClass();
	~TurretClass();
	//void HoldTurret();
	void SetTarg(float v);
	void UpdateTurret(float Input,int kinecttoggle, float kinectangle, float kinectbankangle);
	void Reset_Sensors();
	void SendData();
	void GetData();
};
#endif

