/*
 * main.cpp
 *
 *  Created on: Feb 26, 2020
 *      Author: raltmeyer
 *
 *  Main code with setup() e loop() functions
 *
 *  Sample commands
 *   FWVR
 *   RDBK:0010
 *   WRBK:0010:00:11:22:33:44:55:66:77:88:99
 *   CLBK:0000:002F
 */

#include <Arduino.h>
#include "28c_programmer.h"
#include "pin_defs.h"

const static char msg_start[] PROGMEM = "#START";

//*********
//*
void setup() {
	pinMode(PIN_ROM_OE, INPUT_PULLUP);
	pinMode(PIN_ROM_WE, INPUT_PULLUP);

	// To solve a wrong wire on the first board version
	// jumper on the board between these pins:
	pinMode(A6, INPUT); //(DO NOT USE) A6 -> D8
	pinMode(A7, INPUT); //(DO NOT USE) A7 -> D7

	digitalWrite(PIN_ROM_OE, HIGH);
	digitalWrite(PIN_ROM_WE, HIGH);
	digitalWrite(PIN_SHIFTREG_SER, LOW);
	digitalWrite(PIN_SHIFTREG_RCLK, LOW);
	digitalWrite(PIN_SHIFTREG_SRCLK, LOW);

	pinMode(PIN_ROM_OE, OUTPUT);
	pinMode(PIN_ROM_WE, OUTPUT);
	pinMode(PIN_SHIFTREG_SER, OUTPUT);
	pinMode(PIN_SHIFTREG_RCLK, OUTPUT);
	pinMode(PIN_SHIFTREG_SRCLK, OUTPUT);

	// Setup of data pins
	setDataPinsMode(DATA_PIN_MODE_READ);

	// Start serial comm
	Serial.begin(57600);
	Serial.println(getProgMemString(msg_start));
}

//*********
//*
void loop() {

	cmd_processor();

}

