#include "Ramp.h"


#define ramp_servoBegTime .75f
#define ramp_lower_time (.6f + ramp_servoBegTime)
#define ramp_raise_time (.6f + ramp_servoBegTime)
#define ramp_end_delay_time (.4f + ramp_lower_time)
#define ramp_servoEndTime (1.6f + ramp_end_delay_time)//2.0

#define ramp_servo_lock_angle .78f//.78
#define ramp_servo_open_angle .1f

RampClass::RampClass()
{

	Tomahawk = new Relay(8,Relay::kBothDirections);
	Lock = new Servo(10);
	
	State_Timer = new Timer();
	current_timer= 0.0f;
	previous_timer = 0.0f;

	CurrentToggleBtn = 0;
	PrevToggleBtn = 0;
	state = 0;
}
void RampClass::SetState(int s)
{
	state = s;
	State_Timer->Reset();
	State_Timer->Start();
	current_timer = 0.0f;
	previous_timer = 0.0f;
}

void RampClass::Update_Raise()
{
	if(current_timer < ramp_servoBegTime)
	{
		Lock->Set(ramp_servo_open_angle);
	}
	else if(current_timer < ramp_raise_time)
	{
		Tomahawk->Set(Relay::kReverse);
	}
	else 
	{
		SetState(ramp_Up);
		Tomahawk->Set(Relay::kOff);
	}
}
void RampClass::Update_Lower()
{
	if(current_timer < ramp_servoBegTime)
	{
		Lock->Set(ramp_servo_open_angle);
	}
	else if(current_timer < ramp_lower_time)
	{
		Tomahawk->Set(Relay::kForward);
	}
	else if(current_timer < ramp_end_delay_time)
	{
		Tomahawk->Set(Relay::kOff);
	}
	else if(current_timer < ramp_servoEndTime)
	{
		Lock->Set(ramp_servo_lock_angle);
		Tomahawk->Set(Relay::kOff);
	}
	else 
	{
		SetState(ramp_Down);
		Tomahawk->Set(Relay::kOff);
	}
}
void RampClass::Update_Ramp(int ToggleBTNState,int raise, int lower,float testserv)
{
	current_timer = State_Timer->Get();
	PrevToggleBtn = CurrentToggleBtn;
	CurrentToggleBtn = ToggleBTNState;
//	Lock->Set(testserv);
	if((raise == 1)&& (lower == 1))
	{
		Tomahawk->Set(Relay::kOff);
	}
	else if ((raise == 0 )&&(lower == 0))
	{
		Tomahawk->Set(Relay::kOff);
	}
	else if ((raise == 1 )&&(lower == 0))
	{
		Tomahawk->Set(Relay::kReverse);
	}
	else if ((raise == 0 )&&(lower == 1))
	{
		Tomahawk->Set(Relay::kForward);
	}
	if((CurrentToggleBtn == 1)&&(PrevToggleBtn == 0))
	{
		if(state != ramp_Raising)
		{
			if(state != ramp_Lowering)
			{
				if(state == ramp_Up)
				{
					SetState(ramp_Lowering);
				}
				else if(state == ramp_Down)
				{
					SetState(ramp_Raising);
				}
			}
		}
		
	}
	switch (state) 
	{
		case ramp_Up:
		case ramp_Down:
			break;
		case ramp_Raising:
			Update_Raise();
			break;
		case ramp_Lowering:
			Update_Lower();
			break;
			
	}
//	Lock->Set(0.50f);
	previous_timer = current_timer;
}
void RampClass::SendData()
{
	SmartDashboard *Dash = SmartDashboard::GetInstance();
	Dash->PutDouble("servo",Lock->Get());
}


