#!/usr/bin/env python3

from datetime import datetime
import logging
import time
import threading
import signal
import sys
import re

from src.board_controller import BoardController
from src.serial_handler import SerialHandler
from ..src.power_profiler import PowerProfiler

from influxdb import InfluxDBClient

logging.basicConfig(level=logging.INFO)
log = logging.getLogger()

re_prog = None

DEV_0_ID = "0"
DEV_1_ID = "1"
DEV_2_ID = "2"
DEV_3_ID = "3"
POWER_MEASUREMENT_DURATION = 10

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
    json_body = []
    if len(uart_data) != 0:
        for line in uart_data:
            try:
                # parse line to separate fields
                result = re_prog.match(line)
                groups = result.groups()
                # print(groups)
                # ('00:01:47.625,305', 'dbg', 'sensors_izo', 'read_reg', 'Error on i2c_read()')
                # ('00:04:17.938,873', 'inf', '', 'main', 'sample: 14')
                raw = result.group(0)
                # print(raw)
                dto = datetime.strptime(groups[0].replace(',', ''), '%H:%M:%S.%f')
                ts = str(dto.time())
                # print(ts)
                lvl = groups[1]
                # print(lvl)
                if len(groups[2]) == 0:
                    module = groups[3]
                    function = None
                else:
                    module = groups[2]
                    function = groups[3]
                # print(module)
                # print(function)
                text = groups[4]
                # print(text)
                json_obj = {
                    "measurement": "log-test-vid",
                    "tags": {
                        "devId": device_id,
                        "ts": ts,
                        "lvl": lvl,
                        "module": module,
                        # "function": function,
                    },
                    "fields": {
                        "uart_out": text,
                        "uart_raw": raw
                    }
                }
                if function != None:
                    json_obj["tags"]["function"] = function
            except Exception as ex:
                # log.warning(ex)
                json_obj = {
                    "measurement": "log-test-vid",
                    "tags": {
                        "devId": device_id,
                    },
                    "fields": {
                        "uart_raw": str(line)
                    }
                }

            json_body.append(json_obj)

        # "time": get_current_timestamp() if added, data doesn't show in grafana dashboard
        # print(json_body)
        result = influx_client.write_points(json_body)
        # log.info("Write data result: " + str(result))


def prepare_log_parse():
    global re_prog
    # compile regular expression to be reused

    # [00:01:47.625,305] <dbg> sensors_izo.read_reg: Error on i2c_read()
    # [HH:MM:SS.???,???] <lvl> MODULE.FUNCTION: LOG_MSG
    # [00:04:17.938,873] <inf> main: sample: 14
    # [HH:MM:SS.???,???] <lvl> MODULE: LOG_MSG
    re_prog = re.compile("\[(.*)\].*<(.*)> ([^\.]*?)\.?([^\.]*?): (.*)")


def send_influxdb_power(average_power_consumption_mWh, average_current_mA, minimum_current_mA, maximum_current_mA):
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
    # disable power output from ppk
    power_profiler.disable_power()
    # turn off power for all targets on nrf-test-tool
    for i in range(4):
        ret = board_controller.set_power(i, "off")
        if not ret:
            log.error("Unable to turn off power on target " + str(i))


def device_uart_thread_fun(uart_dev, device_id):
    log.info("usb uart init: " + uart_dev)
    ser = SerialHandler(uart_dev, 115200, timeout=2)
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

def device_measure_power(power_prof, device_id, measurement_duration):
    # power_prof object, device id (0-3), measurement_duration in seconds
    while True:
        power_prof.start_measuring()
        log.info("Measuring started")
        time.sleep(measurement_duration)
        power_prof.stop_measuring()
        log.info("Measuring stopped")

        average_power_consumption_mWh = power_prof.get_average_power_consumption_mWh()
        average_current_mA = power_prof.get_average_current_mA()
        minimum_current_mA = power_prof.get_min_current_mA()
        maximum_current_mA = power_prof.get_max_current_mA()

        send_influxdb_power(average_power_consumption_mWh, average_current_mA, minimum_current_mA, maximum_current_mA)


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

# init power profiler
log.info("init power ")
power_profiler = PowerProfiler("/dev/ttyACM3", 3000)
power_profiler.enable_power()   # enable output power

# init influxdb connection
log.info("influx init")
influx_client = init_influxdb()

# register the handler for SIGTERM and SIGINT signal to terminate this program
signal.signal(signal.SIGTERM, signal_handler)
signal.signal(signal.SIGINT, signal_handler)

# compile regex parser
prepare_log_parse()

# init threads
log.info("init threads")
th0 = threading.Thread(target=device_uart_thread_fun, args=("/dev/ttyUSB0", DEV_0_ID,), daemon=True)
th1 = threading.Thread(target=device_uart_thread_fun, args=("/dev/ttyUSB1", DEV_1_ID,), daemon=True)
th2 = threading.Thread(target=device_uart_thread_fun, args=("/dev/ttyUSB2", DEV_2_ID,), daemon=True)
th3 = threading.Thread(target=device_uart_thread_fun, args=("/dev/ttyUSB3", DEV_3_ID,), daemon=True)
th_pw = threading.Thread(target=device_measure_power, args=(power_profiler, POWER_MEASUREMENT_DURATION, ), daemon=True)

# start threads
log.info("starting threads")
th0.start()
th1.start()
th2.start()
th3.start()
th_pw.start()

while True:
    time.sleep(1)

program_exit()
