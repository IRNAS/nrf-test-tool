import unittest
import time

from src.serial_handler import SerialHandler

# class TestCommands(unittest.TestCase):

#     @classmethod
#     def setUpClass(cls):
#         cls.ser = SerialHandler(serial_port="/dev/ttyACM0", baudrate=115200, timeout=2)

#     def test_ads1015(self):
#         """Test ads1015"""
#         self.ser.write("test ads")

#         reads = [None] * 4
#         reads[0] = self.ser.read_until_starts_with("ADC TEST TARGET 0")
#         reads[1] = self.ser.read_until_starts_with("ADC TEST TARGET 1")
#         reads[2] = self.ser.read_until_starts_with("ADC TEST TARGET 2")
#         reads[3] = self.ser.read_until_starts_with("ADC TEST TARGET 3")

#         for read in reads:
#             adc_values = [i for i, x in enumerate(read.split(": ")) if isinstance(x, int)]
#             print(adc_values)
#             if read is None:
#                 return False

#         return True

#     def test_max14661(self):
#         """Test max14661"""
#         self.ser.write("test max")

#         ret = None
#         ret = self.ser.read_until_starts_with("MAX TEST")

#         if ret:
#             return True

#         return False

#     def test_tca6424(self):
#         """Test tca6424"""
#         self.ser.write("test tca")

#         ret = None
#         ret = self.ser.read_until_starts_with("MAX TEST")

#         if ret:
#             return True

#         return False

ser = SerialHandler(serial_port="/dev/ttyACM0", baudrate=115200, timeout=2)
ser.write("test ads")

read = ser.ser.readlines()
print(read)

reads = [None] * 4
reads[0] = ser.read_until_starts_with("ADC TEST TARGET 0")
reads[1] = ser.read_until_starts_with("ADC TEST TARGET 1")
reads[2] = ser.read_until_starts_with("ADC TEST TARGET 2")
reads[3] = ser.read_until_starts_with("ADC TEST TARGET 3")

for read in reads:
    adc_values = [i for i, x in enumerate(read.split(": ")) if isinstance(x, int)]
    print(adc_values)