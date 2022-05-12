#!/usr/bin/env python3
import serial
import time

class SerialHandler:

    def __init__(self, serial_port, baudrate, timeout=50):
        self.ser = serial.Serial(serial_port, baudrate, timeout=timeout)
        self.timeout = timeout

    def __del__(self):
        try:
            if self.ser:
                self.ser.close()
                self.ser = None
        except Exception as e:
            raise e

    def close_ser(self):
        try:
            if self.ser:
                self.ser.close()
                self.ser = None
        except Exception as e:
            raise e

    def read(self):
        #print(f"Receiving on {self.ser}")
        received = self.ser.readline()
        # print("RS: ", received)
        return received.decode("utf-8").strip()

    def write(self, data):
        data += "\n"
        #print(f"Writing on {self.ser}")
        # print("WS: ", data)
        self.ser.write(data.encode())

    def read_until(self, s):
        to = 0
        read_start_time = time.time()
        while True:
            rx = self.read()
            if rx == s:
                return
            if rx == "":
                to += 1
                if to > 10:
                    raise Exception("Serial timed out")
            if time.time() - read_start_time > self.timeout:
                raise Exception(f"Didn't read {s} in {self.timeout} seconds")

    def read_until_starts_with(self, s, timeout=10, debug=False):
        to = 0
        start_time = time.time()
        while True:
            try:
                rx = self.read()
                if debug:
                    print(rx)
                if len(rx) >= len(s) and rx[0:len(s)] == s:
                    return rx
            except Exception as e:
                print(e)
            # print(rx)
            # print(s)
            # if rx == "":
            #     to += 1
            #     if to > 10:
            #         raise Exception("Serial timed out - retries")
            if time.time() - start_time > timeout:
                raise Exception("Serial timed out - timeout")

    def read_until_contains(self, s, timeout=10, debug=False):
        to = 0
        start_time = time.time()
        while True:
            rx = self.read()
            if debug:
                print(rx)
            # print(s)
            if len(rx) >= len(s) and s in rx:
                return rx
            if rx == "":
                to += 1
                if to > 10:
                    raise Exception("Serial timed out")
            if time.time() - start_time > timeout:
                raise Exception("Serial timed out")

    def read_until_contains_either(self, s, timeout=10, debug=False):
        to = 0
        start_time = time.time()
        while True:
            rx = self.read()
            if debug:
                print(rx)
            # print(s)
            for string in s:
                if len(rx) >= len(string) and string in rx:
                    return rx
            if rx == "":
                to += 1
                if to > 10:
                    raise Exception("Serial timed out")
            if time.time() - start_time > timeout:
                raise Exception("Serial timed out")

    def read_until_starts_with_either(self, a, b, debug=False, timeout=10):
        to = 0
        start_time = time.time()
        while True:
            rx = self.read()
            if debug:
                print(f"Read: {rx}")
            if (len(rx) >= len(a) and rx[0:len(a)] == a) or (len(rx) >= len(b) and rx[0:len(b)] == b):
                return rx
            # if rx == "":
            #     to += 1
            #     if to > 10:
            #         raise Exception("Serial timed out. Did you flash code with debug enabled?")
            if time.time() - start_time > timeout:
                raise Exception("Serial timed out")

    def read_until_starts_with_one_of(self, vars, timeout=10):
        to = 0
        start_time = time.time()
        while True:
            rx = self.read()
            # print(f"Read: {rx}")
            for var in vars:
                if len(rx) >= len(var) and rx[0:len(var)] == var:
                    # print(f"Found match")
                    return rx
            # if rx == "":
            #     to += 1
            #     if to > 10:
            #         raise Exception("Serial timed out. Did you flash code with debug enabled?")
            if time.time() - start_time > timeout:
                raise Exception("Serial timed out")
