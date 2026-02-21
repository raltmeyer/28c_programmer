#!/usr/bin/env python3

import sys
import serial
import argparse
import struct
import os

# Show the progress bar
def progressbar(progress, message):
    barsize = int(round(30*progress))
    text = "\rProcessing: [{0}] {1} ".format( "#"*barsize + "-"*(30-barsize), message)
    sys.stdout.write(text)
    sys.stdout.flush()

parser = argparse.ArgumentParser(description='28C EEPROM Programmer')
parser.add_argument('serial', type=str, help='Serial port')
parser.add_argument('mode', type=str, choices=['write', 'read', 'clear'], help='Mode operation')
parser.add_argument('--file', type=str, dest='file', help='File name')
parser.add_argument('--address', type=lambda x: int(x,0), dest='address_begin', default=0x0000, help='Start address')
parser.add_argument('--address-end', type=lambda x: int(x,0), dest='address_end', default=0x0FFF, help='End address')
args = parser.parse_args()

# Open serial connection
ser = serial.Serial(
    port=args.serial,
    baudrate=57600,
    timeout=5
)
print ('Serial port   : ' + ser.name)
print ('Serial status : ' + str(ser.isOpen()))
print ('Address Start : ' + format(args.address_begin, '04X'))

# Wait the start message from board
s = ser.read_until().decode().rstrip()

# Interface for the read command
if (args.mode == 'read'):
	if (not args.file):
		print ("*Read mode must have a file")
		quit()
	print ('Address End   : ' + format(args.address_end, '04X'))
	print ('Mode          : Read from EEPROM')

	with open (args.file, 'wb') as file:
		for address in range(args.address_begin, args.address_end, 16):
			#create the command message
			cmd = "RDBK:" + format(address, '04X') + "\r"

			#send to the board and wait response
			ser.write(cmd.encode())
			line_read = ser.read_until().decode().rstrip()
			
			if not line_read:
				continue
				
			if line_read[0] == "#":
				print ("\n" + line_read)
				continue

			progressbar((address - args.address_begin)/(args.address_end - args.address_begin), "Block " + format(address, '04X'))

			#write data received to the file
			line_striped = line_read.split(':')
			for data in line_striped[1:]:
				if data:
					file.write(struct.pack("=B",int(data,16)))

	file.close()
	progressbar(1, "Done")
	print()	
	ser.close()
	quit()

# Interface for the write command
if (args.mode == 'write'):
	if (not args.file):
		print ("Write mode must have a file")
		quit()

	address_end = args.address_begin + os.path.getsize(args.file)
	print ('Address End   : ' + format(address_end, '04X'))
	print ('Mode          : Write to EEPROM')

	address = args.address_begin
	with open (args.file, "rb") as file:

		while True:
			data_read = file.read(16)
			if data_read:
				#create the command message
				data_write = "WRBK:" + format(address, '04X')
				for data_b in data_read:
					data_write += ":" + format(int(data_b), '02X')
				data_write += "\r"

				progressbar((address - args.address_begin)/(address_end - args.address_begin), "Block " + format(address, '04X'))
	
				#send to the board and wait response
				ser.write(data_write.encode())
				while True:
					serial_ret = ser.read_until().decode().rstrip()
					if not serial_ret:
						continue
					
					if serial_ret[0] == "#":
						if serial_ret != "#WRBK_OK":
							print ("\n" + serial_ret)
						break

				#prepare address for next block
				address += 16

			else: #End of file
				break
		file.close()
	progressbar(1, "Done")
	print()	
	ser.close()
	quit()

# Interface for the Clear command
if (args.mode == 'clear'):
	print ('Address End   : ' + format(args.address_end, '04X'))
	print ('Mode          : Clear EEPROM')
	cmd = "CLBK:" + format(args.address_begin, '04X') + ":" + format(args.address_end, '04X') + "\r"
	ser.write(cmd.encode())
	address = args.address_begin
	while True:
		line_read = ser.read_until().decode().rstrip()
		if not line_read:
			continue
		
		if line_read[0] == "#":
			if line_read == "#CLBK_DONE":
				break				
			print("\n" + line_read)
			continue 
		
		try:
			address = int(line_read, 16)
			progressbar((address - args.address_begin) / max(1, args.address_end - args.address_begin), "Block " + format(address, '04X'))
		except ValueError:
			pass
	progressbar(1, "Done")
	print()
	ser.close()
	quit()

# Nothing to do, close the serial connection and quit
ser.close()
quit()

