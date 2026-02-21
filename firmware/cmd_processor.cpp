/*
 * cmd_processor.c
 *
 *  Created on: Feb 26, 2020
 *      Author: raltmeyer
 *
 *  Parser for command processor
 *
 */

#include <Arduino.h>
#include "28c_programmer.h"

#define CMD_BUFFER 100
char cmd_buffer[CMD_BUFFER];
const static char msg_notfound[] PROGMEM = "#ERROR_COMMAND_NOT_FOUND";
const static char msg_toolong[] PROGMEM = "#ERROR_COMMAND_LINE_TOO_LONG";
const static char msg_cmdshort[] PROGMEM = "#ERROR_COMMAND_MUST_HAVE_4_CHARS";

//Struct with commands
typedef struct commands_s {
	char command_text[5];
	void (*command)(char[]);
};
const commands_s commands[] = {
/*********************/
	{ "CLBK", &cmd_clear_block },
	{ "WRBK", &cmd_write_block },
	{ "RDBK", &cmd_read_block },
	{ "RDBT", &cmd_read_byte },
	{ "FWVR", &cmd_get_version }
/*********************/
};
int commands_qtd = sizeof(commands) / sizeof(commands_s);

//*********
//*
void cmd_processor() {
	if (Serial.available()) {
		int bytes_read = Serial.readBytesUntil('\r', cmd_buffer, CMD_BUFFER - 1); //Read line from serial
		cmd_buffer[bytes_read] = '\0'; // Null-terminate string
		
		if (bytes_read < 4) { //min command size
			Serial.println(getProgMemString(msg_cmdshort));
			return;
		}

		if (bytes_read >= CMD_BUFFER - 1) { //Command line is too large
			Serial.println(getProgMemString(msg_toolong));
			return;
		}

		for (int cnt = 0; cnt < commands_qtd; cnt++) { //Search for command
			if (!strncmp(cmd_buffer, commands[cnt].command_text, 4)) {
				//Execute a function call
				(*commands[cnt].command)(cmd_buffer);
				return;
			}
		}

		//Returns error if no command was found
		Serial.println(getProgMemString(msg_notfound));
	}
}

