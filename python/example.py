import logging
import time
from src.board_controller import BoardController

logging.basicConfig(format='%(asctime)s - %(levelname)s - %(message)s', datefmt='%d/%m/%Y %H:%M:%S', level=int(10))

board_controller = BoardController("/dev/ttyACM0", 115200, 2)

"""Practical example displaying control of one target"""

# power target 0 from ppk
ret = board_controller.set_power(0, "ppk")
if not ret:
    logging.error("Unable to enable ppk power on target 0")

# set jtag channel 0 (nrf91 for example)
ret = board_controller.set_jtag(0)
if not ret:
    logging.error("Unable to set jtag channel 0")

# read adc value on channel 0 and 1
ret = board_controller.read_adc(0)
if not ret:
    logging.error("Unable to read adc on channel 0")

ret = board_controller.read_adc(1)
if not ret:
    logging.error("Unable to read adc on channel 1")

# reset target after pgoramming
ret = board_controller.reset(0)
if not ret:
    logging.error("Unable to reset target 0")

# power off target 0
ret = board_controller.set_power(0, "off")
if not ret:
    logging.error("Unable to power off target 0")
