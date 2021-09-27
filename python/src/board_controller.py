import time
import logging

from .serial_handler import SerialHandler

class PowerCommandEnum():
    ON = "on"
    OFF = "off"
    PPK = "ppk"
    BLINK = "blink"

class RelayEnum():
    CHARGE = "charge"
    BATTERY = "battery"
class JtagDeviceEnum():
    NRF52 = 0
    NRF91 = 1
    DISABLE = -1

class LedColorEnum():
    GREEN = "green"
    RED = "red"
    ORANGE = "orange"

class PinEnum():
    TEST_1 = 0
    TEST_2 = 1
    TEST_3 = 2
    TEST_4 = 3

class AdcGainEnum():
    V6 = 0x0000
    V4 = 0x0200
    V2 = 0x0400
    V1 = 0x0600
    V05 = 0x0800
    V025 = 0x0A00

class BoardController():
    def __init__(self, serial_port, baudrate, timeout, lock):
        self.ser = None
        try:
            self.ser = SerialHandler(serial_port=serial_port, baudrate=baudrate, timeout=timeout)
        except Exception as e:
            print(e)
        self.lock = lock

        for t in range(0, 4):
            self.set_power(t, PowerCommandEnum.OFF)
            self.control_led_on_target(t, PowerCommandEnum.OFF, None)
            self.set_adc_gain(t, AdcGainEnum.V6)
            self.control_relay_on_target(t, "off", RelayEnum.BATTERY)
            self.control_relay_on_target(t, "off", RelayEnum.CHARGE)

    def set_uart(self, state):
        """Set all FTDI uart lines to ON/OFF"""
        self.lock.acquire()
        if type(state) != str or (state != PowerCommandEnum.ON and state != PowerCommandEnum.OFF):
            logging.warning(f'Invalid state: {state}. Valid states are "on", "off"')
            return False

        self.ser.write(f"uart {state}")

        ret = self.ser.read_until_starts_with_either("OK", "ERROR")
        self.lock.release()
        if ret == "OK":
            logging.info(f"Successfully toggled uart: {ret}")
            return True
        if ret == "ERROR":
            logging.info(f"Failed to read from serial for cmd set_uart: {ret}")
            return False

    def set_power(self, target, state):
        """Set power to target 0-3, state = on/off/ppk"""
        self.lock.acquire()  # acquire lock
        if type(target) != int or target < 0 or target > 3:
            logging.warning(f"Invalid target: {target}. Valid targets: 0-3.")
            return False

        if type(state) != str or (state != PowerCommandEnum.ON and state != PowerCommandEnum.OFF and state != PowerCommandEnum.PPK):
            logging.warning(f'Invalid state: {state}. Valid states are "on", "off" and "ppk"')
            return False

        self.ser.write(f"power {target} {state}")

        ret = self.ser.read_until_starts_with_either("OK", "ERROR")
        self.lock.release()  # release lock
        if ret == "OK":
            logging.info(f"Successfully set power: {ret}")
            return True
        if ret == "ERROR":
            logging.error(f"Failed to read from serial for cmd set_power: {ret}")
            return False

    def set_jtag(self, channel):
        """Enable jtag to channel 0-7 or disable all with -1"""
        self.lock.acquire()  # acquire lock
        if type(channel) != int or channel < -1 or channel > 7:
            logging.warning(f"Invalid channel: {channel}. Valid channels: 0-7 or -1 for all off")
            return False

        self.ser.write(f"jtag {channel}")

        ret = self.ser.read_until_starts_with_either("OK", "ERROR")
        self.lock.release()  # release lock
        if ret == "OK":
            #logging.info(f"Successfully set jtag: {ret}")
            return True
        if ret == "ERROR":
            logging.error(f"Failed to read from serial for cmd: {ret}")
            return False

    def set_jtag_on_target(self, target, jtag_device):
        """Enable jtag for jtag_device (nrf91 or nrf52) on desired target (0-3)"""
        self.lock.acquire()  # acquire lock
        if type(target) != int or target < 0 or target > 3:
            logging.warning(f"Invalid target: {target}. Valid targets: 0-3!")
            return False

        channel = target * 2 + jtag_device
        self.ser.write(f"jtag {channel}")

        ret = self.ser.read_until_starts_with_either("OK", "ERROR")
        self.lock.release()  # release lock
        if ret == "OK":
            #logging.info(f"Successfully read from serial for cmd: {ret}")
            return True
        if ret == "ERROR":
            logging.error(f"Failed to read from serial for cmd: {ret}")
            return False

    def reset(self, channel):
        """Reset channel 0-7"""
        self.lock.acquire()  # acquire lock
        if type(channel) != int or channel < 0 or channel > 7:
            logging.warning(f"Invalid channel: {channel}. Valid channels: 0-7.")
            return False

        self.ser.write(f"reset {channel}")
        ret = self.ser.read_until_starts_with_either("OK", "ERROR")
        self.lock.release()  # release lock
        if ret == "OK":
            #logging.info(f"Successfully read from serial for cmd: {ret}")
            return True
        if ret == "ERROR":
            logging.error(f"Failed to read from serial for cmd: {ret}")
            return False

    def reset_target(self, target):
        """Reset target"""
        self.lock.acquire()  # acquire lock
        if type(target) != int or target < 0 or target > 3:
            logging.warning(f"Invalid channel: {target}. Valid targets: 0-3.")
            return False

        channel = target * 2
        self.ser.write(f"reset {channel}")
        try:
            ret = self.ser.read_until_starts_with_either("OK", "ERROR")
        except Exception as e:
            self.lock.release()
            return False
        time.sleep(0.05)
        channel += 1
        self.ser.write(f"reset {channel}")
        try:
            ret = self.ser.read_until_starts_with_either("OK", "ERROR")
        except Exception as e:
            self.lock.release()
            return False
        self.lock.release()  # release lock
        if ret == "OK":
            #logging.info(f"Successfully read from serial for cmd: {ret}")
            return True
        if ret == "ERROR":
            logging.error(f"Failed to read from serial for cmd: {ret}")
            return False

    def read_adc(self, channel):
        """Read adc on channel 0-15"""
        self.lock.acquire()  # acquire lock
        if type(channel) != int or channel < 0 or channel > 15:
            logging.warning(f"Invalid channel: {channel}. Valid channels: 0-15.")
            return False

        self.ser.write(f"adc {channel}")
        read_line = self.ser.read_until_starts_with("Read analog value")
        voltage = read_line.split(": ")[1]
        ret = self.ser.read_until_starts_with_either("OK", "ERROR")
        self.lock.release()  # release lock
        if ret == "OK":
            #logging.info(f"Successfully read adc value from serial: {ret}")
            return int(voltage)
        if ret == "ERROR":
            logging.error(f"Failed to read adc value from serial: {ret}")
            return False

    def set_adc_gain(self, target, gain):
        """Set adc gain on target"""
        self.lock.acquire()  # acquire lock
        if type(target) != int or target < 0 or target > 3:
            logging.warning(f"Invalid target: {target}. Valid targets: 0-3.")
            return False

        self.ser.write(f"gain {target} {gain}")
        ret = self.ser.read_until_starts_with_either("OK", "ERROR")
        self.lock.release()  # release lock
        if ret == "OK":
            #logging.info(f"Successfully read adc value from serial: {ret}")
            return True
        if ret == "ERROR":
            logging.error(f"Failed to read adc value from serial: {ret}")
            return False

    def read_adc_on_target(self, target, pin):
        """Read adc on target 0-3, selected pin (TEST_1 is 0, TEST_2 is 1, TEST_3 is 2, TEST_4 is 3)"""
        self.lock.acquire()  # acquire lock
        if type(target) != int or target < 0 or target > 3:
            logging.warning(f"Invalid target: {target}. Valid targets: 0-3.")
            return False

        channel = target * 4 + pin

        self.ser.write(f"adc {channel}")
        read_line = self.ser.read_until_starts_with("Read analog value")
        voltage = read_line.split(": ")[1]
        ret = self.ser.read_until_starts_with_either("OK", "ERROR")
        self.lock.release()  # release lock
        if ret == "OK":
            # logging.info(f"Successfully read adc value from serial: {ret}")
            return int(voltage)
        if ret == "ERROR":
            logging.error(f"Failed to read adc value from serial: {ret}")
            return False

    def detect_device(self, target):
        """Detect device on channel 0-3"""
        try:
            self.lock.acquire()  # acquire lock
            if type(target) != int or target < 0 or target > 3:
                logging.warning(f"Invalid target: {target}. Valid targets: 0-3.")
                return False

            self.ser.write(f"detect {target}")
            time.sleep(0.05)
            read_line = self.ser.read_until_starts_with("Detected board")
            board_detected = read_line.split(": ")[1]
            ret = self.ser.read_until_starts_with_either("OK", "ERROR")
            self.lock.release()  # release lock
            if ret == "OK":
                #logging.info(f"Successfully read device present from serial: {ret}. ")
                return board_detected
            if ret == "ERROR":
                logging.error(f"Failed to read from serial for cmd: {ret}")
                return False
        except Exception as e:
            self.lock.release()
            return None

    def control_led(self, channel, state):
        """Control led on channel 0-7, state = on/off"""
        self.lock.acquire()  # acquire lock
        if type(channel) != int or channel < 0 or channel > 7:
            logging.warning(f"Invalid channel: {channel}. Valid channels: 0-7.")
            return False

        if type(state) != str or (state != PowerCommandEnum.ON and state != PowerCommandEnum.OFF):
            logging.warning(f'Invalid state: {state}. Valid states are "on", "off')
            return False

        self.ser.write(f"led {channel} {state}")

        ret = self.ser.read_until_starts_with_either("OK", "ERROR")
        self.lock.release()  # release lock
        if ret == "OK":
            logging.info(f"Successfully read from serial for cmd: {ret}")
            return True
        if ret == "ERROR":
            logging.error(f"Failed to read from serial for cmd: {ret}")
            return False

    def control_led_on_target(self, target, state, color):
        """Control led on target 0-3, color=red/green, state = on/off"""
        self.lock.acquire()  # acquire lock
        if type(target) != int or target < 0 or target > 3:
            logging.warning(f"Invalid target: {target}. Valid targets: 0-3!")
            self.lock.release()  # release lock
            return False

        if type(state) != str or (state != PowerCommandEnum.ON and state != PowerCommandEnum.OFF and state != PowerCommandEnum.BLINK):
            logging.warning(f'Invalid state: {state}. Valid states are "on", "off" and "blink"')
            self.lock.release()  # release lock
            return False

        if color is not None:
            if type(color) != str or (color != LedColorEnum.GREEN and color != LedColorEnum.RED and color != LedColorEnum.ORANGE):
                logging.warning(f'Invalid color: {color}. Valid colors are "green", "red" and "orange"')
                self.lock.release()  # release lock
                return False

        #channel = target * 2 + color
        #print("Controlling led with: {}".format(f"led {channel} {state}"))
        if color:
            self.ser.write(f"led {target} {state} {color}")
        else:
            self.ser.write(f"led {target} {state}")
        try:
            ret = self.ser.read_until_starts_with_either("OK", "ERROR")
            self.lock.release()  # release lock
        except:
            self.lock.release()
            return False
        if ret == "OK":
            #logging.info(f"Successfully read from serial for cmd: {ret}")
            return True
        if ret == "ERROR":
            #logging.error(f"Failed to read from serial for cmd: {ret}")
            return False
    
    def control_relay_on_target(self, target, state, relay):
        """Control relay on target 0-3, relay=battery/charge, state=on/off"""
        self.lock.acquire()  # acquire lock
        if type(target) != int or target < 0 or target > 3:
            logging.warning(f"Invalid target: {target}. Valid targets: 0-3")
            self.lock.release()  # release lock
            return False

        if type(state) != str or (state != PowerCommandEnum.ON and state != PowerCommandEnum.OFF):
            logging.warning(f'Invalid state: {state}. Valid states are "on", "off"')
            self.lock.release()  # release lock
            return False

        if relay is not None:
            if type(relay) != str or (relay != RelayEnum.CHARGE and relay != RelayEnum.BATTERY):
                logging.warning(f'Invalid relay: {relay}. Valid relays are "charge" and "battery"')
                self.lock.release()  # release lock
                return False

        self.ser.write(f"relay {target} {state} {relay}")
        try:
            ret = self.ser.read_until_starts_with_either("OK", "ERROR")
            self.lock.release()  # release lock
        except:
            self.lock.release()
            return False
        if ret == "OK":
            # logging.info(f"Successfully read from serial for cmd: {ret}")
            return True
        if ret == "ERROR":
            # logging.error(f"Failed to read from serial for cmd: {ret}")
            return False