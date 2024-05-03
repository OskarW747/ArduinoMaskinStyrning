#include "Arduino.h"
#include "Button.h"
#include "EEPROM.h"


/*
 * Inkoppling
 * Öppning av ventiler kopplas på pinne 2 - 5
 * Stängning av ventiler kopplas på pinne 6-9
 * Öppning av ventil 1 på pinne 2, stängning på pinne 6, osv
 *
 * Knappar kopplas på pinne A0 - A4
 *
 * Knapp 0 - 1: höj/sänk pistoler
 * Knapp 2 - 3: Starta pistoler
 * Knapp 4 - 5: Stoppa pistoler
 * Knapp 6 - 7: Återställ cykler
 */

// Customization
#define CAST_TIME				10000
#define TRAVEL_DOWN_TIME		5000
#define PRESSURE_RELIEF_TIME	2000
#define MAX_CYCLES				20

// Automation states
#define OFF						0
#define LOWER					1
#define CAST					2
#define LIFT					3
#define START					4

// Button pins
#define BUTTON_PIN_0			A0
#define BUTTON_PIN_1			A4
#define BUTTON_PIN_2			A1
#define BUTTON_PIN_3			A5
#define BUTTON_PIN_4			A2
#define BUTTON_PIN_5			A6
#define BUTTON_PIN_6			A3
#define BUTTON_PIN_7			A7

// LED diod pins
#define FILL_PISTOL_1_LED_PIN	10
#define FILL_PISTOL_2_LED_PIN	11

// Relay pins
#define LIFT_VALVE_1_PIN		2
#define LIFT_VALVE_2_PIN		3
#define PISTOL_VALVE_1_PIN		4
#define PISTOL_VALVE_2_PIN		5
#define CLOSE_VALVE_OFFSET_PIN	4 // Offset bc open valve 1 = pin 2; close valve 1 = pin 6

#define NUM_PISTOLS				2

#define DOWN 					0
#define UP						1

// EEPROM addresses
#define PISTOL_1_CYCLE_ADDRESS	0
#define PISTOL_2_CYCLE_ADDRESS	1

// Array för alla utgångspinnar
const uint8_t relayPins[] = {
		LIFT_VALVE_1_PIN,
		LIFT_VALVE_2_PIN,
		PISTOL_VALVE_1_PIN,
		PISTOL_VALVE_2_PIN,
		LIFT_VALVE_1_PIN + CLOSE_VALVE_OFFSET_PIN,
		LIFT_VALVE_2_PIN + CLOSE_VALVE_OFFSET_PIN,
		PISTOL_VALVE_1_PIN + CLOSE_VALVE_OFFSET_PIN,
		PISTOL_VALVE_2_PIN + CLOSE_VALVE_OFFSET_PIN};

Button button0(BUTTON_PIN_0);
Button button1(BUTTON_PIN_1);
Button button2(BUTTON_PIN_2);
Button button3(BUTTON_PIN_3);
Button button4(BUTTON_PIN_4);
Button button5(BUTTON_PIN_5);
Button button6(BUTTON_PIN_6);
Button button7(BUTTON_PIN_7);

struct pistol {
	bool automatic;
	uint8_t state; // state in the automation cycle
	uint8_t lastState;
	uint8_t position;
	unsigned long startTime;
	uint8_t cycles;
};

// Variable for resetting program after start
bool starting = true;

// Initiate pistol structs
pistol pistols[NUM_PISTOLS] = {true, OFF, OFF, DOWN, NULL, 0};

void setup() {



	// Initiate button-pins
	button0.begin();
	button1.begin();
	button2.begin();
	button3.begin();
	button4.begin();
	button5.begin();
	button6.begin();
	button7.begin();



	// Initiate Serial Monitor
	Serial.begin(9600);
	Serial.println("Arduino Connected");

	updateButtons();

	// Initiate relay-pins
	for (uint8_t i = 0; i <= sizeof(relayPins); i++) {
		pinMode(relayPins[i], OUTPUT);
		digitalWrite(relayPins[i], HIGH); // Set to HIGH to turn off relays
	}


}

void loop() {


	updateButtons();





	automation();

	// reset program after start
	if (starting == true && millis() >= 100) {
		for (uint8_t i = 0; i < NUM_PISTOLS; i++) {
			pistols[i].state = LIFT; // Sets the valves to the default starting position
			pistols[i].lastState = CAST;
			starting = false;
			Serial.println("Program reset");
		}
	}
}

/*
 * Cycle sequence
 */
void automation() {

	for (uint8_t i = 0; i < NUM_PISTOLS; i++) {

			switch (pistols[i].state) {
				case (OFF):
					if (pistols[i].state != pistols[i].lastState) {
						pistols[i].lastState = pistols[i].state;
						Serial.print("Pistol off: ");
						Serial.println(i);
					}


					break;
				case (LOWER):
					if (pistols[i].state != pistols[i].lastState) {
						Serial.print("Pistol lowering: ");
						Serial.println(i);


						closeValve(LIFT_VALVE_1_PIN + i); // + i To get offset if the program is on pistol 2
						pistols[i].startTime = millis();
						pistols[i].lastState = pistols[i].state;
					} else if ((millis() - pistols[i].startTime) >= TRAVEL_DOWN_TIME) {
						Serial.print("Pistol lowered: ");
						Serial.println(i);

						pistols[i].position = DOWN;

						if (pistols[i].automatic == true) {
							pistols[i].state = CAST;
						} else {
							pistols[i].state = OFF;
						}
					}

					break;
				case (CAST):
					if (pistols[i].state != pistols[i].lastState) {
						Serial.print("Starting cast: ");
						Serial.println(i);

						openValve(PISTOL_VALVE_1_PIN + i);
						pistols[i].startTime = millis();
						pistols[i].lastState = pistols[i].state;
						pistols[i].cycles++;
					} else if ((millis() - pistols[i].startTime) >= CAST_TIME) {
						pistols[i].state = LIFT;
					}

					break;
				case (LIFT):
					if (pistols[i].automatic == false) {
						Serial.print("Lifting pistol: ");
						Serial.println(i);

						openValve(LIFT_VALVE_1_PIN + i);
						pistols[i].state = OFF;
						pistols[i].position = UP;
					} else if (pistols[i].state != pistols[i].lastState) {
						closeValve(PISTOL_VALVE_1_PIN + i);
						// Depressurize before lifting
						if (pistols[i].lastState == CAST) {
							Serial.print("Depressurizing pistol: ");
							Serial.println(i);

							pistols[i].startTime = millis();
							pistols[i].lastState = pistols[i].state;
						}

					} else if ((millis() - pistols[i].startTime) >= PRESSURE_RELIEF_TIME) {
						Serial.print("Lifting pistol: ");
						Serial.println(i);

						openValve(LIFT_VALVE_1_PIN + i);
						pistols[i].state = OFF;
						pistols[i].position = UP;
					}
					break;
			}



	}

}

/*
 * Check if any of the buttons are pressed
 */
void updateButtons() {
	if (button0.pressed()) {
		Serial.println("Button 0 pressed");

		if (pistols[0].position == UP) {
			pistols[0].state = LOWER;
			pistols[0].automatic = false;
		} else {
			pistols[0].state = LIFT;
			pistols[0].automatic = false;
		}

	}

	if (button1.pressed()) {
		Serial.println("Button 1 pressed");
		if (pistols[1].position == UP) {
			pistols[1].state = LOWER;
			pistols[1].automatic = false;
		} else {
			pistols[1].state = LIFT;
			pistols[1].automatic = false;
		}
	}

	if (button2.pressed()) {
		Serial.println("Button 2 pressed");

		if (pistols[0].position != DOWN) {
			pistols[0].state = LOWER;
			pistols[0].automatic = true;
		} else {
			pistols[0].state = CAST;
			pistols[0].automatic = true;
		}
	}

	if (button3.pressed()) {
		Serial.println("Button 3 pressed");

		if (pistols[1].position != DOWN) {
			pistols[1].state = LOWER;
			pistols[1].automatic = true;
		} else {
			pistols[1].state = CAST;
			pistols[1].automatic = true;
		}
	}

	if (button4.pressed()){
		Serial.println("Button 4 pressed");

		pistols[0].state = LIFT;
	}

	if (button5.pressed()) {
		Serial.println("Button 5 pressed");

		pistols[1].state = LIFT;
	}

	if (button6.pressed()) {
		Serial.println("Button 6 pressed");
	}

	if (button7.pressed()) {
		Serial.println("Button 7 pressed");
	}



}



/*
 * Functions to handle opening and closing of valves
 */
void openValve(uint8_t valve) {
	if (millis() >= 100) {
		Serial.print("Valve opened: ");
		Serial.println(valve - 1);

		digitalWrite(valve, LOW);
		digitalWrite(valve + CLOSE_VALVE_OFFSET_PIN, HIGH);
	}


}

void closeValve(uint8_t valve) {
	if (millis() >= 100) {
		Serial.print("Valve closed: ");
		Serial.println(valve - 1);

		digitalWrite(valve, HIGH);
		digitalWrite(valve + CLOSE_VALVE_OFFSET_PIN, LOW);
	}

}

