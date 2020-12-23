import logging
import time
from src.board_controller import BoardController

from influxdb import InfluxDBClient


# Server data
host = 'grafana.irnas.eu'
user = 'vid_user'
password = 'vid_pass12345'

client = InfluxDBClient(host=host, port=8086, username=user, password=password)

client.switch_database('Izoelektro_ram')

