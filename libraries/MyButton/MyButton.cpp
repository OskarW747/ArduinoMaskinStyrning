/*
 * MyButton.cpp
 *
 *  Created on: 22 apr. 2024
 *      Author: oskwal05
 */
#include "Arduino.h"
#include "MyButton.h"

MyButton::MyButton(uint8_t pin)
	:	_pin(pin)
{

}

void MyButton::begin(){

	pinMode(_pin, INPUT_PULLUP);

}


