#!/usr/bin/python
# -*- coding: utf-8 -*-
import sys
import serial
import glob
import time

command = None
commands = ["Enable/diable payload (stored in EEPROM)", "Enable/disable test LED", "Get chip AVcc voltage (mV)", "Execute payload", "Dump EEPROM", "Exit"]

def print_commands():
    global commands, command

    for item in enumerate(commands):
        print "[%d] %s" % item

    try:
        command = int(raw_input("Command: "))
    except (ValueError, IndexError, KeyboardInterrupt, EOFError):
        print "\n\n> You fail at typing numbers/number out of range.\n"
        print_commands()
        pass


if __name__ == '__main__':

    print " ███▄    █  ▒█████  ▄▄▄█████▓ ██▀███   ██▓ ▄████▄   ██ ▄█▀  ██████"
    print " ██ ▀█   █ ▒██▒  ██▒▓  ██▒ ▓▒▓██ ▒ ██▒▓██▒▒██▀ ▀█   ██▄█▒ ▒██    ▒"
    print " ▓██  ▀█ ██▒▒██░  ██▒▒ ▓██░ ▒░▓██ ░▄█ ▒▒██▒▒▓█    ▄ ▓███▄░ ░ ▓██▄"
    print " ▓██▒  ▐▌██▒▒██   ██░░ ▓██▓ ░ ▒██▀▀█▄  ░██░▒▓▓▄ ▄██▒▓██ █▄   ▒   ██▒"
    print " ▒██░   ▓██░░ ████▓▒░  ▒██▒ ░ ░██▓ ▒██▒░██░▒ ▓███▀ ░▒██▒ █▄▒██████▒▒"
    print " ░ ▒░   ▒ ▒ ░ ▒░▒░▒░   ▒ ░░   ░ ▒▓ ░▒▓░░▓  ░ ░▒ ▒  ░▒ ▒▒ ▓▒▒ ▒▓▒ ▒ ░"
    print " ░ ░░   ░ ▒░  ░ ▒ ▒░     ░      ░▒ ░ ▒░ ▒ ░  ░  ▒   ░ ░▒ ▒░░ ░▒  ░ ░"
    print "    ░   ░ ░ ░ ░ ░ ▒    ░        ░░   ░  ▒ ░░        ░ ░░ ░ ░  ░  ░"
    print "         ░     ░ ░              ░      ░  ░ ░      ░  ░         ░"
    print

    ports = glob.glob('/dev/tty.*')

    result = []
    for port in ports:
        try:
            s = serial.Serial(port)
            s.close()
            result.append(port)
        except (OSError, serial.SerialException):
            pass

    for item in enumerate(result):
        print "[%d] %s" % item

    try:
        idx = int(raw_input("Select: "))
        port = result[idx]
        print ""
    except (ValueError, IndexError, KeyboardInterrupt, EOFError):
        print "\n\n> You fail at typing numbers/number out of range.\n"
        sys.exit(1)

    ser = serial.Serial()
    ser.port = port
    ser.baudrate = 9600
    ser.open()

    time.sleep(0.5)

    line = []

    while 1:
        print_commands()

        if 5 == command:
            print "Bye!"
            ser.close()
            sys.exit(0)

        try:
            ser.timeout = 1
            ser.write(str(command))
            ser.flush()

            # response
            #while True:
            #    for c in ser.read():
            #        line.append(c)
            #        if c == '\n':
            #            print("".join([str(i) for i in line]))
            #            line = []
            #            break

            print("\n< %s" % (ser.readline()))

            if 3 == command:
                print "Sleeping 5.."
                time.sleep(5)

        except (Exception):
            print "Error while working serial."
            ser.close()
            raise
            sys.exit(1)

