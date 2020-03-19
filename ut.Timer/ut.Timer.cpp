#include "pch.h"
#include "CppUnitTest.h"
#include "..\utils\Timer.h"
#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace av::utils;

void fun()
{
	Logger::WriteMessage("Timeout happened!");
}

TEST_CLASS(utTimer)
{
public:

	//========================================================================================================================!
	TEST_METHOD(BasicTimeout)
	{
		Timer t;
		t.setTimeout(fun, std::chrono::seconds(2));
		std::this_thread::sleep_for(std::chrono::seconds(3));
	}

	//========================================================================================================================!
	TEST_METHOD(IntervalTimeout)
	{
		Timer t;
		t.setInterval(fun, std::chrono::seconds(1));
		std::this_thread::sleep_for(std::chrono::seconds(4));
	}
};
