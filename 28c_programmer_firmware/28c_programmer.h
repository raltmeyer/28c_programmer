/*
 * 28c_programmer.h
 *
 *  Created on: Feb 26, 2020
 *      Author: raltmeyer
 */

#define DATA_PIN_MODE_READ 0
#define DATA_PIN_MODE_WRITE 1

//cmd_processor.cpp
void cmd_processor();

//interface.cpp
void setDataPinsMode(boolean);
void flashWrite(int, byte);
byte flashRead(int);
void SDPmode(int);

//util.cpp
char* getProgMemString(const char*);
int checkAddress(int);

//cmd_*.cpp
void cmd_write_block(char[]);
void cmd_read_block(char[]);
void cmd_read_byte(char[]);
void cmd_clear_block(char[]);
void cmd_get_version(char[]);

