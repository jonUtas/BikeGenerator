#include <gui/screen1_screen/Screen1View.hpp>

#include "../../../../STM32CubeIDE/Application/User/globals.h"

//extern "C"
//{    // another way
//	bool test;
//};

Screen1View::Screen1View()
{

}

void Screen1View::setupScreen()
{
    Screen1ViewBase::setupScreen();
}

void Screen1View::tearDownScreen()
{
    Screen1ViewBase::tearDownScreen();
}

void Screen1View::ButtonClick()
{
	test = !test;
	globalCount++;

}

void Screen1View::setLight (bool state)
{

}

void Screen1View::setADC (int val)
{
	gauge1.setValue(val);
	gauge1.invalidate();
}
