#include "Arduino.h"
#include "Button.h"


/*
 * Inkoppling
 * Öppning av ventiler kopplas på pinne 2 - 5
 * Stängning av ventiler kopplas på pinne 6-9
 * Öppning av ventil 1 på pinne 2, stängning på pinne 6, osv
 *
 * Knappar kopplas på pinne 9-13
 */


// Button pins
#define BUTTON_PIN_1		10
#define BUTTON_PIN_2		11
#define BUTTON_PIN_3		12
#define BUTTON_PIN_4		13

// Relay-pins
const uint8_t numOfRelayPins = 7;
const uint8_t firstRelayPin = 2;
const uint8_t lastRelayPin = 9;
const uint8_t relayPins[numOfRelayPins];
bool relayStates[numOfRelayPins];

Button button1(BUTTON_PIN_1);
Button button2(BUTTON_PIN_2);
Button button3(BUTTON_PIN_3);
Button button4(BUTTON_PIN_4);

void setup() {

	// Initiate button-pins
	button1.begin();
	button2.begin();
	button3.begin();
	button4.begin();

	// initiate relay-pins
	for (uint8_t i = 0; i < numOfRelayPins; i++) {
		pinMode(relayPins[(i + firstRelayPin)], OUTPUT);
	}
}

void loop() {

	if (button1.pressed())
		button1Pressed();
	if (button2.pressed())
		button2Pressed();
	if (button3.pressed())
		button3Pressed();
	if (button4.pressed())
		button4Pressed();




	// Update relays
	for (uint8_t i = 0; i < numOfRelayPins; i++) {
		digitalWrite(relayPins[(i + firstRelayPin)], relayStates[i]);
	}
}

void liftPistol(uint8_t pistol){
	uint8_t open = pistol;
	uint8_t close = pistol + (numOfRelayPins/2);
	relayStates[open] = HIGH;
	relayStates[close] = LOW;
}

void lowerPistol(uint8_t pistol){
	uint8_t open = pistol;
	uint8_t close = pistol + (numOfRelayPins/2);
	relayStates[open] = LOW;
	relayStates[close] = HIGH;
}

void button1Pressed(){
	liftPistol(0);
}

void button2Pressed(){

}

void button3Pressed(){

}

void button4Pressed(){

}
