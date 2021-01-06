import time
import logging

from src.serial_handler import SerialHandler

class CommandEnum():
    ON = "on"
    OFF = "off"
    PPK = "ppk"

class BoardController():
    def __init__(self, serial_port, baudrate, timeout):
        self.ser = SerialHandler(serial_port=serial_port, baudrate=baudrate, timeout=timeout)

    def set_power(self, target, state):
        """Set power to target 0-3, state = on/off/ppk"""
        if type(target) != int or target < 0 or target > 3:
            logging.warning(f"Invalid target: {target}. Valid targets: 0-3.")
            return False

        if type(state) != str or (state != CommandEnum.ON and state != CommandEnum.OFF and state != CommandEnum.PPK):
            logging.warning(f'Invalid state: {state}. Valid states are "on", "off" and "ppk"')
            return False

        self.ser.write(f"power {target} {state}")

        ret = self.ser.read_until_starts_with_either("OK", "ERROR")
        if ret == "OK":
            logging.info(f"Successfully read from serial: {ret}")
            return True
        if ret == "ERROR":
            logging.error(f"Failed to read from serial: {ret}")
            return False

    def set_jtag(self, channel):
        """Enable jtag to channel 0-7 or disbale all with off"""
        if type(channel) != int or channel < -1 or channel > 7:
            logging.warning(f"Invalid channel: {channel}. Valid channels: 0-7 or -1 for all off")
            return False

        self.ser.write(f"jtag {channel}")

        ret = self.ser.read_until_starts_with_either("OK", "ERROR")
        if ret == "OK":
            logging.info(f"Successfully read from serial: {ret}")
            return True
        if ret == "ERROR":
            logging.error(f"Failed to read from serial: {ret}")
            return False

    def reset(self, channel):
        """Reset channel 0-7"""
        if type(channel) != int or channel < 0 or channel > 7:
            logging.warning(f"Invalid channel: {channel}. Valid channels: 0-7.")
            return False

        self.ser.write(f"reset {channel}")

        ret = self.ser.read_until_starts_with_either("OK", "ERROR")
        if ret == "OK":
            logging.info(f"Successfully read from serial: {ret}")
            return True
        if ret == "ERROR":
            logging.error(f"Failed to read from serial: {ret}")
            return False

    def read_adc(self, channel):
        """Read adc on channel 0-15"""
        if type(channel) != int or channel < 0 or channel > 15:
            logging.warning(f"Invalid channel: {channel}. Valid channels: 0-15.")
            return False

        self.ser.write(f"adc {channel}")

        read_line = self.ser.read_until_starts_with("(mV): ")
        voltage = read_line.split(": ")[1]
        ret = self.ser.read_until_starts_with_either("OK", "ERROR")
        if ret == "OK":
            logging.info(f"Successfully read from serial: {ret}")
            return voltage
        if ret == "ERROR":
            logging.error(f"Failed to read from serial: {ret}")
            return False

    def control_led(self, channel, state):
        """Control led on channel 0-7, state = on/off"""
        if type(channel) != int or channel < 0 or channel > 7:
            logging.warning(f"Invalid channel: {channel}. Valid channels: 0-7.")
            return False

        if type(state) != str or (state != CommandEnum.ON and state != CommandEnum.OFF):
            logging.warning(f'Invalid state: {state}. Valid states are "on", "off')
            return False

        self.ser.write(f"led {channel} {state}")

        ret = self.ser.read_until_starts_with_either("OK", "ERROR")
        if ret == "OK":
            logging.info(f"Successfully read from serial: {ret}")
            return True
        if ret == "ERROR":
            logging.error(f"Failed to read from serial: {ret}")
            return False