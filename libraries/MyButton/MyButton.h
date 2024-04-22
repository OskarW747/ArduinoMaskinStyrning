/*
 * MyButton.h
 *
 *  Created on: 22 apr. 2024
 *      Author: oskwal05
 */

#ifndef MyButton_h
#define MyButton_h
#include "Arduino.h"

class MyButton {

public:
	MyButton(uint8_t pin);
	void begin();


private:
	uint8_t _pin;

};



#endif /* LIBRARIES_MYBUTTON_MYBUTTON_H_ */
