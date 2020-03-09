/*
 * interface.cpp
 *
 *  Created on: Feb 26, 2020
 *      Author: raltmeyer
 *
 *   Interface between arduino and EEPROM/latches
 *
 */

#include <Arduino.h>
#include "28c_programmer.h"
#include "pin_defs.h"

char PINS_DATA[] = { PIN_DATA_D0, PIN_DATA_D1, PIN_DATA_D2, PIN_DATA_D3,
					PIN_DATA_D4, PIN_DATA_D5, PIN_DATA_D6, PIN_DATA_D7 };

//*********
//* Set the address A0 to A15 through the latches
void setAddress(int address) {
	// send all 16 address bits and trigger the latch to store
	shiftOut(PIN_SHIFTREG_SER, PIN_SHIFTREG_SRCLK, MSBFIRST, (address >> 8));
	shiftOut(PIN_SHIFTREG_SER, PIN_SHIFTREG_SRCLK, MSBFIRST, address);
	digitalWrite(PIN_SHIFTREG_RCLK, LOW);
	digitalWrite(PIN_SHIFTREG_RCLK, HIGH);
	digitalWrite(PIN_SHIFTREG_RCLK, LOW);
}

//*********
//* Set the data on the pins of eeprom
void setData(byte data) {
	for (int n = 0; n < 8; n += 1) {
		digitalWrite(PINS_DATA[n], data & 1);
		data = data >> 1;
	}
}

//*********
//* Get the data avaiable on the eeprom data pins
byte getData(void) {
	byte data = 0;
	for (int n = 7; n >= 0; n -= 1) {
		data = data << 1;
		data = data + digitalRead(PINS_DATA[n]);
	}
	return data;
}

//*********
//* Change the pinmode on data pins on nano
void setDataPinsMode(boolean readWrite) {
	digitalWrite(PIN_ROM_WE, HIGH); //disable Write
	digitalWrite(PIN_ROM_OE, HIGH); //disable Read

	for (int n = 0; n < 8; n += 1) {
		if (readWrite == DATA_PIN_MODE_WRITE) {
			pinMode(PINS_DATA[n], OUTPUT);
		} else {
			pinMode(PINS_DATA[n], INPUT);
		}
	}
}

//*********
//* Do all steps to write data on the eeprom
void flashWrite(int address, byte data) {

	setDataPinsMode(DATA_PIN_MODE_WRITE);
	setAddress(address);
	setData(data);

	//Pulsa para gravar na flash
	digitalWrite(PIN_ROM_WE, LOW);
	delayMicroseconds(1);
	digitalWrite(PIN_ROM_WE, HIGH);
	delay(10);
}

//*********
//* Do all steps to read data on the EEPROM
byte flashRead(int address) {
	byte data = 0;

	setDataPinsMode(DATA_PIN_MODE_READ);
	setAddress(address);

	digitalWrite(PIN_ROM_OE, LOW);
	delayMicroseconds(1);
	data = getData();
	digitalWrite(PIN_ROM_OE, HIGH);

	return data;
}

//*********
//* Similar to flashRead is only used to change the
//* SDP status on the EEPROM
void SDPWrite(int address, byte data) {
	setAddress(address);
	setData(data);
	digitalWrite(PIN_ROM_WE, LOW);
	delayMicroseconds(1);
	digitalWrite(PIN_ROM_WE, HIGH);
	delay(1);
}

//*********
//* Change the SDP (Software Data Protection) status
void SDPmode(int mode) {

	setDataPinsMode(DATA_PIN_MODE_WRITE);
	if (mode) {
		SDPWrite(0x5555, 0xAA);
		SDPWrite(0x2AAA, 0x55);
		SDPWrite(0x5555, 0x80);
		SDPWrite(0x5555, 0xAA);
		SDPWrite(0x2AAA, 0x55);
		SDPWrite(0x5555, 0x20);

		SDPWrite(0x1555, 0xAA);
		SDPWrite(0x0AAA, 0x55);
		SDPWrite(0x1555, 0x80);
		SDPWrite(0x1555, 0xAA);
		SDPWrite(0x0AAA, 0x55);
		SDPWrite(0x1555, 0x20);

	} else {
		SDPWrite(0x5555, 0xAA);
		SDPWrite(0x2AAA, 0x55);
		SDPWrite(0x5555, 0xA0);

		SDPWrite(0x1555, 0xAA);
		SDPWrite(0x0AAA, 0x55);
		SDPWrite(0x1555, 0xA0);
	}

	flashRead(0);
}

