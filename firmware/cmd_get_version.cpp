/*
 * cmd_get_version.cpp
 *
 *  Created on: Mar 02, 2020
 *      Author: raltmeyer
 *
 *  Implements a command
 *
 */

#include <Arduino.h>
#include "28c_programmer.h"

const static char firmware_version[] PROGMEM = "#FIRMWARE_VERSION:1.0";

//*********
//* Return the firmware version
//* FWVR
void cmd_get_version(char buffer[]) {
	Serial.println(getProgMemString(firmware_version));
}

