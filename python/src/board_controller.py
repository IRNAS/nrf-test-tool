import time
import logging

from .serial_handler import SerialHandler

class PowerCommandEnum():
    ON = "on"
    OFF = "off"
    PPK = "ppk"

class JtagDeviceEnum():
    NRF52 = 0
    NRF91 = 1
    DISABLE = -1

class LedColorEnum():
    GREEN = 0
    RED = 1

class BoardController():
    def __init__(self, serial_port, baudrate, timeout):
        self.ser = SerialHandler(serial_port=serial_port, baudrate=baudrate, timeout=timeout)

    def set_power(self, target, state):
        """Set power to target 0-3, state = on/off/ppk"""
        if type(target) != int or target < 0 or target > 3:
            logging.warning(f"Invalid target: {target}. Valid targets: 0-3.")
            return False

        if type(state) != str or (state != PowerCommandEnum.ON and state != PowerCommandEnum.OFF and state != PowerCommandEnum.PPK):
            logging.warning(f'Invalid state: {state}. Valid states are "on", "off" and "ppk"')
            return False

        self.ser.write(f"power {target} {state}")

        ret = self.ser.read_until_starts_with_either("OK", "ERROR")
        if ret == "OK":
            logging.info(f"Successfully read from serial for cmd set_power: {ret}")
            return True
        if ret == "ERROR":
            logging.error(f"Failed to read from serial for cmd set_power: {ret}")
            return False

    def set_jtag(self, channel):
        """Enable jtag to channel 0-7 or disable all with -1"""
        if type(channel) != int or channel < -1 or channel > 7:
            logging.warning(f"Invalid channel: {channel}. Valid channels: 0-7 or -1 for all off")
            return False

        self.ser.write(f"jtag {channel}")

        ret = self.ser.read_until_starts_with_either("OK", "ERROR")
        if ret == "OK":
            logging.info(f"Successfully read from serial for cmd: {ret}")
            return True
        if ret == "ERROR":
            logging.error(f"Failed to read from serial for cmd: {ret}")
            return False

    def set_jtag_on_target(self, target, jtag_device):
        """Enable jtag for jtag_device (nrf91 or nrf52) on desired target (0-3)"""
        if type(target) != int or target < 0 or target > 3:
            logging.warning(f"Invalid target: {target}. Valid targets: 0-3!")
            return False

        channel = target * 2 + jtag_device
        self.ser.write(f"jtag {channel}")

        ret = self.ser.read_until_starts_with_either("OK", "ERROR")
        if ret == "OK":
            logging.info(f"Successfully read from serial for cmd: {ret}")
            return True
        if ret == "ERROR":
            logging.error(f"Failed to read from serial for cmd: {ret}")
            return False

    def reset(self, channel):
        """Reset channel 0-7"""
        if type(channel) != int or channel < 0 or channel > 7:
            logging.warning(f"Invalid channel: {channel}. Valid channels: 0-7.")
            return False

        self.ser.write(f"reset {channel}")

        ret = self.ser.read_until_starts_with_either("OK", "ERROR")
        if ret == "OK":
            logging.info(f"Successfully read from serial for cmd: {ret}")
            return True
        if ret == "ERROR":
            logging.error(f"Failed to read from serial for cmd: {ret}")
            return False

    def read_adc(self, channel):
        """Read adc on channel 0-15"""
        if type(channel) != int or channel < 0 or channel > 15:
            logging.warning(f"Invalid channel: {channel}. Valid channels: 0-15.")
            return False

        self.ser.write(f"adc {channel}")

        ret = self.ser.read_until_starts_with_either("OK", "ERROR")
        if ret == "OK":
            logging.info(f"Successfully read from serial for cmd: {ret}")
            return True  # TODO return actual value insted of True
        if ret == "ERROR":
            logging.error(f"Failed to read from serial for cmd: {ret}")
            return False

    def control_led(self, channel, state):
        """Control led on channel 0-7, state = on/off"""
        if type(channel) != int or channel < 0 or channel > 7:
            logging.warning(f"Invalid channel: {channel}. Valid channels: 0-7.")
            return False

        if type(state) != str or (state != PowerCommandEnum.ON and state != PowerCommandEnum.OFF):
            logging.warning(f'Invalid state: {state}. Valid states are "on", "off')
            return False

        self.ser.write(f"led {channel} {state}")

        ret = self.ser.read_until_starts_with_either("OK", "ERROR")
        if ret == "OK":
            logging.info(f"Successfully read from serial for cmd: {ret}")
            return True
        if ret == "ERROR":
            logging.error(f"Failed to read from serial for cmd: {ret}")
            return False

    def control_led_on_target(self, target, color, state):
        """Control led on target 0-3, color=red/green, state = on/off"""
        if type(target) != int or target < 0 or target > 3:
            logging.warning(f"Invalid target: {target}. Valid targets: 0-3!")
            return False

        if type(state) != str or (state != PowerCommandEnum.ON and state != PowerCommandEnum.OFF):
            logging.warning(f'Invalid state: {state}. Valid states are "on", "off')
            return False

        channel = target * 2 + color
        self.ser.write(f"led {channel} {state}")

        ret = self.ser.read_until_starts_with_either("OK", "ERROR")
        if ret == "OK":
            logging.info(f"Successfully read from serial for cmd: {ret}")
            return True
        if ret == "ERROR":
            logging.error(f"Failed to read from serial for cmd: {ret}")
            return False
