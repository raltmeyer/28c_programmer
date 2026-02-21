/*
 * util.cpp
 *
 *  Created on: Feb 27, 2020
 *      Author: raltmeyer
 *
 *  Util functions
 *
 */

#include <Arduino.h>

char string_ram[40]; // Temporary buffer used to copy strings from arduino flash

//*********
//* Copy string content from the arduino flash (progmem) to RAM
char* getProgMemString(const char *str) {
	strcpy_P(string_ram, (char*) str);
	return string_ram;
}

//*********
//* Verify if the address is valid and inside the range
const static char msg_addressspaceerror[] PROGMEM = "#ERROR_INVALID_ADDRESS";
int checkAddress(int address) {
	if (address < 0 || address > 0x7FFF) { //32k range (28c256 address space)
		Serial.println(getProgMemString(msg_addressspaceerror));
		return 0;
	}
	return 1;
}

