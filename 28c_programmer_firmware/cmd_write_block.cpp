/*
 * cmd_write_block.cpp
 *
 *  Created on: Feb 27, 2020
 *      Author: raltmeyer
 *
 *  Implements a command
 *
 */

#include <Arduino.h>
#include "28c_programmer.h"

const static char msg_ok[] PROGMEM = "#WRBK_OK";
const static char msg_dataerror[] PROGMEM = "#ERROR_DATA_ERROR";

//*********
//* Write a sequence of N bytes stating on address
//* WRBK:address:data1:data2:....:data16
//* WRBK:0000:01:02:...:16
void cmd_write_block(char buffer[]) {
	int param = 0;
	unsigned int address = 0;
	char *tok;

	//Parse command and parameters
	tok = strtok(buffer, ":");
	while ((tok != NULL)) {
		//Address
		if (param == 1) {
			address = strtoul(tok, NULL, 16);

			//Read data com parameter and write to flash
		} else if (param >= 2) {
			byte data;
			if (!checkAddress(address)) {
				return;
			}
			if (strlen(tok) == 0) {
				Serial.println(getProgMemString(msg_dataerror));
			}
			data = strtoul(tok, NULL, 16);

			flashWrite(address, data);
			address++;
		}

		tok = strtok(NULL, ":");
		param++;
	}

	Serial.println(getProgMemString(msg_ok));
}

