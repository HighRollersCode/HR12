// HrScript.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "FakeRobot.h"


int _tmain(int argc, _TCHAR* argv[])
{
	FakeRobot robot;

	robot.Auto();
	robot.OperatorControl();


	return 0;
}

