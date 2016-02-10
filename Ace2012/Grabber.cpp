#include "Grabber.h"

GrabberClass::GrabberClass()
{
	currToggleState = 0;
	prevToggleState = 0;
	
	GrabberOut = new Solenoid(1,8);
	GrabberIn = new Solenoid(1,7);
	
	Grabber_Delay = new Timer();
	
	toggle = 1;
}
void GrabberClass::Engage()
{
	ResetTimers();
	GrabberOut->Set(true);
	GrabberIn->Set(false);
	//engaged = -1;
	if(Grabber_Delay->Get() > 1.0f);
	{
		GrabberOut->Set(false);
		GrabberIn->Set(true);
		//engaged = 1;
	}
}	
/*
void GrabberClass::DisEngage()
{
	ResetTimers();
	GrabberOut->Set(true);
	GrabberIn->Set(false);
	engaged = -1;
	if(Grabber_Delay->Get() > 1.0f);
	{
		GrabberOut->Set(false);
		GrabberIn->Set(true);
		engaged = 0;
	}
}
*/
void GrabberClass::UpdateGrabber(int togglebuttonstate)
{
	prevToggleState = currToggleState;
	currToggleState = togglebuttonstate;

	
	if((currToggleState == 1)&&(prevToggleState == 0))
	{
		toggle = -toggle;
	}
	if(toggle == 1)
	{
		GrabberOut->Set(false);
		GrabberIn->Set(true);
	}
	else
	{
		GrabberOut->Set(true);
		GrabberIn->Set(false);
	}
	//if(currToggleState == 1)
	//{
	//	GrabberOut->Set(false);
	//			GrabberIn->Set(true);
	//}
	//else
	//{
	//	GrabberOut->Set(true);
	//			GrabberIn->Set(false);
	//}
}
void GrabberClass::ResetTimers()
{
	Grabber_Delay->Reset();
	Grabber_Delay->Start();
}
void GrabberClass::SendData()
{
}
