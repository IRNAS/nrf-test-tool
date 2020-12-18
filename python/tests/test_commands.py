import unittest
import time

from src.board_controller import BoardController

class TestCommands(unittest.TestCase):

    @classmethod
    def setUpClass(cls):
        cls.board_controller = BoardController("/dev/ttyACM0", 115200, 2)

    """Test set_power command"""
    def test_set_power(self):
        for target in range(0, 3):
            for state in ["on", "off", "ppk"]:
                ret = self.board_controller.set_power(target, state)
                with self.subTest():
                    self.assertTrue(ret, "Return value must be True!")
                time.sleep(0.1)

    def test_set_power_wrong_state(self):
        for target in range(0, 3):
            for state in ["banana", -4, 512, 0x498, "ž"]:
                ret = self.board_controller.set_power(target, state)
                with self.subTest():
                    self.assertFalse(ret, "Return value must be False!")
                time.sleep(0.1)

    def test_set_power_wrong_channel(self):
        for target in ["banana", -4, 512, 0x498, "ž"]:
            for state in ["on", "off", "ppk"]:
                ret = self.board_controller.set_power(target, state)
                with self.subTest():
                    self.assertFalse(ret, "Return value must be False!")
                time.sleep(0.1)

    """Test set_jtag command"""
    def test_set_jtag(self):
        for channel in range(-1, 8):
            ret = self.board_controller.set_jtag(channel)
            with self.subTest():
                self.assertTrue(ret, "Return value must be True!")
            time.sleep(0.1)

    def test_set_jtag_wront_channel(self):
        for channel in ["banana", -4, 512, 0x498, "ž"]:
            ret = self.board_controller.set_jtag(channel)
            with self.subTest():
                self.assertFalse(ret, "Return value must be True!")
            time.sleep(0.1)

    """Test reset command"""
    def test_reset(self):
        for channel in range(0, 8):
            ret = self.board_controller.reset(channel)
            with self.subTest():
                self.assertTrue(ret, "Return value must be True!")
            time.sleep(0.1)

    def test_reset_wrong_channel(self):
        for channel in ["banana", -4, 512, 0x498, "ž"]:
            ret = self.board_controller.reset(channel)
            with self.subTest():
                self.assertFalse(ret, "Return value must be True!")
            time.sleep(0.1)

    """Test read_adc"""
    def test_adc(self):
        for channel in range(0, 15):
            ret = self.board_controller.read_adc(channel)
            with self.subTest():
                self.assertTrue(ret, "Return value must be True!")
            time.sleep(0.1)

    def test_adc_wrong_channel(self):
        for channel in ["banana", -4, 512, 0x498, "ž"]:
            ret = self.board_controller.read_adc(channel)
            with self.subTest():
                self.assertFalse(ret, "Return value must be True!")
            time.sleep(0.1)

    """Test led command"""
    def test_control_led(self):
        for target in range(0, 7):
            for state in ["on", "off"]:
                ret = self.board_controller.control_led(target, state)
                with self.subTest():
                    self.assertTrue(ret, "Return value must be True!")
                time.sleep(0.1)

    def test_control_led_wrong_state(self):
        for target in range(0, 7):
            for state in ["banana", -4, 512, 0x498, "ž"]:
                ret = self.board_controller.control_led(target, state)
                with self.subTest():
                    self.assertFalse(ret, "Return value must be False!")
                time.sleep(0.1)

    def test_control_led_wrong_channel(self):
        for target in ["banana", -4, 512, 0x498, "ž"]:
            for state in ["on", "off"]:
                ret = self.board_controller.control_led(target, state)
                with self.subTest():
                    self.assertFalse(ret, "Return value must be False!")
                time.sleep(0.1)
