/*
 * cmd_read_byte.cpp
 *
 *  Created on: Mar 01, 2020
 *      Author: raltmeyer
 *
 *  Implements a command
 *
 */

#include <Arduino.h>
#include "28c_programmer.h"

//*********
//* Read one byte from address
//* RDBT:address
//* RDBT:00FF
void cmd_read_byte(char buffer[]) {
	int param = 0;
	char *tok;
	char add_tmp[6];

	//Parse command and parameters
	tok = strtok(buffer, ":");
	while ((tok != NULL)) {
		//Address
		if (param == 1) {
			unsigned int address = 0, data = 0;

			address = strtoul(tok, NULL, 16);
			if (!checkAddress(address)) {
				return;
			}

			sprintf(add_tmp, "%04X", address);
			Serial.print(add_tmp);
			Serial.print(":");

			data = flashRead(address);
			sprintf(add_tmp, "%02X", data);
			Serial.println(add_tmp);
		}

		tok = strtok(NULL, ":");
		param++;
	}
}

