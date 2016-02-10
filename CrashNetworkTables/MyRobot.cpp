#include "WPILib.h"

#define TEST_TRANSACTIONS 0
#define TEST_WRITECRASH 0
#define TEST_FLOATCORRUPTION 1


const int INTEGERSTORECEIVE = 200;
float dashfloat = 0.0f;

class RobotDemo : public SimpleRobot
{
	
public:
	RobotDemo(void){}
	
	void UpdateDash()
	{
		
		SmartDashboard *Dash = SmartDashboard::GetInstance();
		//Send data every frame:
		//for(int i = 0; i < INTEGERSTOSEND; i++)
		//{
		//	char varname[64];
		//	sprintf(varname,"val%d",i);
		//	Dash->PutInt(varname,dataval);
		//	dataval++;
		//}
		
		NetworkTable * tbl = NetworkTable::GetTable("987Table");
#if TEST_WRITECRASH
		
		if (tbl != NULL)
		{
			for(int i = 0; i < INTEGERSTORECEIVE; i++)
			{
				char varname[64];
				sprintf(varname,"val%d",i);
			    int s = tbl->GetInt(varname);
				Dash->PutInt(varname,s);
			}	
		}
#endif

#if TEST_FLOATCORRUPTION
	
		Dash->PutDouble("testdouble",tbl->GetDouble("testdouble"));
		Dash->PutDouble("testdouble2",tbl->GetInt("testdouble2")/1000.0f);
#endif 		
		
#if TEST_TRANSACTIONS
		
		tbl->BeginTransaction();
		
		Dash->PutInt("TransactionInteger",tbl->GetInt("Transaction_Int"));
		Dash->PutInt("TransactionInteger2",tbl->GetInt("Transaction_Int2"));
		
		tbl->EndTransaction();
#endif
	}	
	
	void Autonomous(void){}
	void OperatorControl(void)
	{
		GetWatchdog().SetEnabled(true);
		GetWatchdog().Feed();
		while (IsOperatorControl())
		{
			GetWatchdog().Feed();
			UpdateDash();
			Wait(0.001);			
		}
	}
};

START_ROBOT_CLASS(RobotDemo);

