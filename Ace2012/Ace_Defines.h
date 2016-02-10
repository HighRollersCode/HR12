#define PRACTICE_BOT 0  // change to 0 for real bot

#define Jag_Drive_Left_ 3
#define Jag_Drive_Left_A 4
#define Jag_Drive_Right_ 1
#define Jag_Drive_Right_A 2

#define Jag_Shooter_ 5
#define Jag_Shooter_A 9

#define Jag_Turret 6

#define Jag_Uptake 7

#define Solenoid_HiGear 1
#define Solenoid_LoGear 2
#define Solenoid_LauchOut 3
#define Solenoid_LaunchIn 4
#define Solenoid_StopperOut 5
#define Solenoid_StopperIn 6
#define Solenoid_GrabberIn 7
#define Solenoid_GrabberOut 8


// in any cpp or h file (make sure you include "AceDefines.h"
//#if (PRACTICE_BOT)

//#else

//#endif

//#if (!PRACTICE_BOT)

//#endif

/*//AUTO_2Ball backup bridge approach
 Auto_GYROSTRAIGHT(1.0f,100,0.0f);
	Auto_GYROSTRAIGHT(.2f,650,0.0f);
	Auto_IntakeOn();
		while((Launcher->HoldingBall == false)&&(Running()))
		{
			Auto_System_Update();
			DriveTrain->UpdateEBrake(1,0);
		}
		DriveTrain->UpdateEBrake(0,0);
 */
