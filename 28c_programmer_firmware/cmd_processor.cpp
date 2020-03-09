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
	char command_text[4];
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
	String input_line;

	if (Serial.available()) {
		input_line = Serial.readStringUntil('\r'); //Read line from serial

		if (input_line.length() < 4) { //min command size
			Serial.println(getProgMemString(msg_cmdshort));
			return;
		}

		if (input_line.length() > CMD_BUFFER) { //Command line is too large
			Serial.println(getProgMemString(msg_toolong));
			return;
		}

		input_line.toCharArray(cmd_buffer, CMD_BUFFER, 0);

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

