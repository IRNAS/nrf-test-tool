#!/usr/bin/env python3
import serial
import logging

class SerialHandler:

    def __init__(self, serial_port, baudrate, timeout=2):
        self.ser = serial.Serial(serial_port, baudrate, timeout=timeout)

    def read(self):
        received = self.ser.readline()
        return received.decode("utf-8").rstrip()

    def write(self, data):
        logging.info(f"Writing: {data}")
        data += "\n"
        self.ser.write(data.encode())

    def read_until(self, s):
        to = 0
        while True:
            rx = self.read()
            if rx == s:
                return
            if rx == "":
                to += 1
                if to > 10:
                    raise Exception("Serial timed out")

    def read_until_starts_with(self, s):
        to = 0
        while True:
            rx = self.read()
            if len(rx) >= len(s) and rx[0:len(s)] == s:
                return rx
            if rx == "":
                to += 1
                if to > 10:
                    raise Exception("Serial timed out")

    def read_until_starts_with_either(self, a, b):
        to = 0
        while True:
            rx = self.read()
            if len(rx) >= len(a) and rx[0:len(a)] == a or len(rx) >= len(b) and rx[0:len(b)] == b:
                return rx
            if rx == "":
                to += 1
                if to > 10:
                    raise Exception("Serial timed out. Did you flash code with debug enabled?")