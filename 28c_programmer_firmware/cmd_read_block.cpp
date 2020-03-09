/*
 * cmd_read_block.cpp
 *
 *  Created on: Feb 27, 2020
 *      Author: raltmeyer
 *
 *  Implements a command
 *
 */

#include <Arduino.h>
#include "28c_programmer.h"

//*********
//* Read a 16 bytes sequence starting from address
//* RDBK:address
//* RDBK:00FF
void cmd_read_block(char buffer[]) {
	int param = 0;
	char *tok;
	char add_tmp[6];

	//Parse command and parameters
	tok = strtok(buffer, ":");
	while ((tok != NULL)) {
		//Address
		if (param == 1) {
			unsigned int address_begin = 0, address = 0, data = 0;
			address_begin = strtoul(tok, NULL, 16);
			if (!checkAddress(address_begin)) {
				return;
			}

			sprintf(add_tmp, "%04X", address_begin);
			Serial.print(add_tmp);

			for (address = address_begin; address < (address_begin + 16); address++) {
				data = flashRead(address);
				sprintf(add_tmp, "%02X", data);
				Serial.print(":");
				Serial.print(add_tmp);
			}
			Serial.println();
		}

		tok = strtok(NULL, ":");
		param++;
	}
}

