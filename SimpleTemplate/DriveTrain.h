#include "WPILib.h"


class DriveTrainClass
{
	int current_AutonTicks;
	
	Jaguar *LeftDrive;
	Jaguar *LeftDrive_a;
	Jaguar *RightDrive;
	Jaguar *RightDrive_a;
	
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
	int ToggleState;
	bool neutral;
	bool was_neutral;
	string gear;
public:	
	int LeftSideTicks;
	int RightSideTicks;
	
	bool Highgear;
	bool Lowgear;
	
	DriveTrainClass();
	
	~DriveTrainClass();
	void UpdateEBrake(int enable);
	void Drive_Auton(float Forward, float Turn);
	void Drive_AutonEncoder(float Forward, float Turn, int Ticks);
	void Drive_AutonTimer(float Forward, float Turn, float Seconds);
	void Drive_Operator(float lY, float rY);
	void ResetEncoders_Timers();
	void Shifter_Update(bool ShifterEnable,int Neutralbtn);
	void SendData();	
};
