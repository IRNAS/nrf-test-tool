#!/usr/bin/env python3

from datetime import datetime
import logging
import time
import threading
import signal
import sys

from src.board_controller import BoardController
from src.serial_handler import SerialHandler

from influxdb import InfluxDBClient

logging.basicConfig(level=logging.INFO)
log = logging.getLogger()

def get_current_timestamp():    
    # format: "2018-03-30T8:02:00Z"
    now = datetime.now()
    dt_string = now.strftime("%Y-%m-%dT%H:%M:%SZ")
    return dt_string

def init_influxdb():
    # Server data
    host = 'grafana.irnas.eu'
    user = 'vid_user'
    password = 'vid_pass12345'

    client = InfluxDBClient(host=host, port=8086, username=user, password=password)

    client.switch_database('database_test')
    return client

def send_influxdb(uart_data, device_id):
    # send test data to influx db
    
    #uart_data = "heh"
    #device_id = "123456789"

    json_body = []
    if len(uart_data) != 0:
        for line in uart_data:
            json_body.append(
                {
                    "measurement": "log-test-vid",
                    "tags": {
                        "device-id": device_id
                    },
                    "fields": {
                        "uart_out": line
                    }
                }
            )
    
        # "time": get_current_timestamp() if added, data doesn't show in grafana dashboard

        result = influx_client.write_points(json_body)
        #log.info("Write data result: " + str(result))

def signal_handler(signal, frame):
    """Handler method for OS signal SIGTERM, clean resources and exit program gracefully."""
    log.warning("SIGTERM received, exiting program.")
    program_exit()
    sys.exit(0)

def program_exit():
    # turn off power for all targets on nrf-test-tool
    for i in range(4):
        ret = board_controller.set_power(i, "off")
        if not ret:
            log.error("Unable to turn off power on target " + str(i))

def device_uart_thread_fun(uart_line, device_id):
    log.info("usb uart init: " + uart_line)
    ser = SerialHandler(uart_line, 115200, timeout=2)
    time.sleep(1)

    while True:
        try:
            line = ser.read()
        except Exception as ex:
            log.warning(ex)
            continue

        if len(line) != 0:
            uart_data = [line]
            send_influxdb(uart_data, device_id)


# --------------------------- Main code ---------------------------
log.info("nrf test tool logger from all uarts to grafana")

# nrf-test-tool init
log.info("nrf test tool init")
board_controller = BoardController("/dev/ttyACM0", 115200, 2)
# power on target 0 from ppk
ret = board_controller.set_power(0, "ppk")
if not ret:
    log.error("Unable to enable ppk power on target 0")

# power all the other three targets from dev board
ret = board_controller.set_power(1, "on")
if not ret:
    log.error("Unable to enable on power on target 0")

ret = board_controller.set_power(2, "on")
if not ret:
    log.error("Unable to enable on power on target 0")
ret = board_controller.set_power(3, "on")
if not ret:
    log.error("Unable to enable on power on target 0")

# init influxdb connection
log.info("influx init")
influx_client = init_influxdb()

# register the handler for SIGTERM and SIGINT signal to terminate this program
signal.signal(signal.SIGTERM, signal_handler)
signal.signal(signal.SIGINT, signal_handler)

# init threads
log.info("init threads")
th0 = threading.Thread(target=device_uart_thread_fun, args=("/dev/ttyUSB0", 0,), daemon=True)
th1 = threading.Thread(target=device_uart_thread_fun, args=("/dev/ttyUSB1", 1,), daemon=True)
th2 = threading.Thread(target=device_uart_thread_fun, args=("/dev/ttyUSB2", 2,), daemon=True)
th3 = threading.Thread(target=device_uart_thread_fun, args=("/dev/ttyUSB3", 3,), daemon=True)

# start threads
log.info("starting threads")
th0.start()
th1.start()
th2.start()
th3.start()

while True:
    time.sleep(1)

program_exit()
