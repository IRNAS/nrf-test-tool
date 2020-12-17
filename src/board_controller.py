import time
import logging
from serial_handler import SerialHandler

class CommandEnum():
    ON = "on"
    OFF = "off"
    PPK = "ppk"

class BoardController():
    def __init__(self, serial_port, baudrate, timeout):
        self.ser = SerialHandler(serial_port=serial_port, baudrate=baudrate, timeout=timeout)

    def set_power(self, channel, state):
        """Set power to channel 0-7, state = on/off/ppk"""
        if channel < 0 or channel > 7:
            logging.warning(f"Invalid channel: {channel}. Valid channels: 0-7.")
            return False

        if state != CommandEnum.ON or state != CommandEnum.OFF or state != CommandEnum.PPK:
            logging.warning(f'Invalid state: {state}. Valid states are "on", "off" and "ppk"')
            return False

        self.ser.write(f"power {channel} {state}")

        ret = self.ser.read_until_starts_with_either("OK", "ERROR")
        if ret == "OK":
            return True
        if ret == "ERROR":
            return False

    def set_jtag(self, channel):
        """Enable jtag to channel 0-7 or disbale all with off"""
        if channel < 0 or channel > 7 or channel != CommandEnum.OFF:
            logging.warning(f"Invalid channel: {channel}. Valid channels: 0-7.")
            return False

        self.ser.write(f"jtag {channel}")

        ret = self.ser.read_until_starts_with_either("OK", "ERROR")
        if ret == "OK":
            return True
        if ret == "ERROR":
            return False

    def reset(self, channel):
        """Reset channel 0-7"""
        if channel < 0 or channel > 7:
            logging.warning(f"Invalid channel: {channel}. Valid channels: 0-7.")
            return False

        self.ser.write(f"reset {channel}")

        ret = self.ser.read_until_starts_with_either("OK", "ERROR")
        if ret == "OK":
            return True
        if ret == "ERROR":
            return False

    def read_adc(self, channel):
        """Read adc on channel 0-15"""
        if channel < 0 or channel > 15:
            logging.warning(f"Invalid channel: {channel}. Valid channels: 0-15.")
            return False

        self.ser.write(f"adc {channel}")

        ret = self.ser.read_until_starts_with_either("OK", "ERROR")
        if ret == "OK":
            return True
        if ret == "ERROR":
            return False

    def control_led(self, channel, state):
        """Control led on channel 0-7, state = on/off"""
        if channel < 0 or channel > 7:
            logging.warning(f"Invalid channel: {channel}. Valid channels: 0-7.")
            return False

        if state != CommandEnum.ON or state != CommandEnum.OFF:
            logging.warning(f'Invalid state: {state}. Valid states are "on", "off')
            return False

        self.ser.write(f"led {channel} {state}")

        ret = self.ser.read_until_starts_with_either("OK", "ERROR")
        if ret == "OK":
            return True
        if ret == "ERROR":
            return False