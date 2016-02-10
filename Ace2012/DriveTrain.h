#ifndef DRIVETRAIN_H
#define DRIVETRAIN_H

#include "WPILib.h"


class DriveTrainClass
{
	int current_AutonTicks;
	int EncoderFailCount;

	int PrevLEnc;
	int PrevREnc;
	Victor *LeftDrive;
	Victor *LeftDrive_a;
	Victor *RightDrive;
	Victor *RightDrive_a;
	
	Encoder *LeftDriveEnc;
	Encoder *RightDriveEnc;
	
	Solenoid *ShifterHigh;
	Solenoid *ShifterLow;
	Solenoid *NeutralHigh;
	Solenoid *NeutralLow;
	
	Timer *Neutral_DelayTM;
	
	int CurrentBrakeBTN;
	int PrevBrakeBTN;
	bool CurrentShifterToggleTrig;
	bool PrevShifterToggleTrig;
	
	bool NeutralBut;
	bool PrevNeutralBut;
	
	bool neutral;
	bool was_neutral;
	string gear;
	
public:	
	int TickingEncoders;
	int LeftSideTicks;
	int RightSideTicks;
	int ToggleState;
	bool Highgear;
	bool Lowgear;
	
	DriveTrainClass();
	
	~DriveTrainClass();
	void UpdateEBrake(int enable, int targ);
	void Drive_Auton(float Forward, float Turn);
	
	void Drive_Operator(float lY, float rY);
	void ResetEncoders_Timers();
	int GetLeftEncoder(){return LeftDriveEnc->Get();}
	int GetRightEncoder(){return RightDriveEnc->Get();}
	
	void Shifter_Update(bool ShifterEnable,int Neutralbtn);
	void SendData();	
	
	void Failsafe_Update();
	void Failsafe_Reset();
};
#endif
