/*
 * cmd_clear_block.cpp
 *
 *  Created on: Feb 27, 2020
 *      Author: raltmeyer
 *
 *  Implements a command
 *
 */

#include <Arduino.h>

#include "28c_programmer.h"

const static char msg_ok[] PROGMEM = "#CLBK_DONE";
const static char msg_paramcount[] PROGMEM = "#ERROR_PARAMETER_COUNT";
const static char msg_paramlarger[] PROGMEM	= "#ERROR_END_PARAMETER_MUST_BE_LARGER";

//*********
//* Write FF to address block
//* CLBK:address_begin:address_end
//* CLBK:0000:0100
void cmd_clear_block(char buffer[]) {
	int param = 0;
	unsigned int address_begin = 0, address_end = 0, address = 0;
	char *tok;

	//Parse command and parameters
	tok = strtok(buffer, ":");
	while ((tok != NULL)) {
		//Address begin
		if (param == 1) {
			address_begin = strtoul(tok, NULL, 16);

			//Address end
		} else if (param == 2) {
			address_end = strtoul(tok, NULL, 16);

		}
		tok = strtok(NULL, ":");
		param++;
	}

	//Check the correct number os parameters
	if (param != 3) { //2 on last loop
		Serial.println(getProgMemString(msg_paramcount));
		return;
	}
	//Check if begin and end address are valid
	if (!checkAddress(address_begin) && !checkAddress(address_end)) {
		return;
	}
	//Check the sequence of addresses
	if (address_begin >= address_end) {
		Serial.println(getProgMemString(msg_paramlarger));
		return;
	}

	//disable data protection
	SDPmode(0);

	//Write 0xFF on the address space
	for (address = address_begin; address < address_end + 1; address++) {
		flashWrite(address, 0xFF);
		if (!(address % 128)) {
			char add_tmp[6];
			sprintf(add_tmp, "%04X", address);
			Serial.println(add_tmp);
		}
	}

	//re-enable data protection
	SDPmode(1);

	Serial.println(getProgMemString(msg_ok));

}

