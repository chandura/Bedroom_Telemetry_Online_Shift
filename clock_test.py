import unittest

from clock_classes import stream_config
from clock_classes import ranges
from clock_functions import write_record
from clock_functions import post_record

from ISStreamer.Streamer import Streamer

configgroup = 'initialstate.test.com'

theranges = ranges(0.00, '00:00', 0.00)
config = stream_config(configgroup)
streamer = Streamer(bucket_name=config.bucketname, bucket_key=config.bucketkey, access_key=config.accesskey)

class TestBetween(unittest.TestCase):

    def test_config(self):
        config = stream_config(configgroup)

        self.assertEqual(config.bucketname, 'Stream Test')

    def test_record_write_1(self):
        write = write_record(1, 100, streamer)
        self.assertEqual(write, 'N')

    def test_record_write_100(self):
        write = write_record(100, 100, streamer)
        self.assertEqual(write, 'Y')

    def test_record_write_500(self):
        write = write_record(500, 100, streamer)
        self.assertEqual(write, 'Y')

    def test_no_record_write_50(self):
        write = write_record(50, 100, streamer)
        self.assertEqual(write, 'N')

    def test_no_record_write_701(self):
        write = write_record(701, 100, streamer)
        self.assertEqual(write, 'N')

    def test_got_data_and_json(self):
        test_json = (str("{\"json\":\"Y\"}"))
        value_to_post, data = post_record('{', 'Y', test_json)
        self.assertEqual(value_to_post, 'Y')

    def test_not_got_jason(self):
        test_json = ("Hello")
        value_to_post, data = post_record('{', 'N', test_json)
        self.assertEqual(value_to_post, 'N')

    def test_reset_ranges(self):
        theranges.temp_min = 20.00
        theranges.temp_max = 20.00
        theranges.ref_date = 15
        theranges.comp_date = 16
        theranges.reset

        self.assertEqual(theranges.temp_min, 50.00)
        self.assertEqual(theranges.temp_max, 0.00)
        self.assertEqual(theranges.ref_date, 16)

    def test_not_reset_ranges(self):
        theranges.temp_min = 20.00
        theranges.temp_max = 20.00
        theranges.ref_date = 16
        theranges.comp_date = 16
        theranges.reset

        self.assertEqual(theranges.temp_min, 20.00)
        self.assertEqual(theranges.temp_max, 20.00)
        self.assertEqual(theranges.ref_date, 16)

    def test_lower_min(self):
        theranges.temp_min = 25.00
        theranges.temp_now = 20.00
        theranges.post_min = 'N'
        theranges.set_min

        self.assertEqual(theranges.temp_min, 20.00)
        self.assertEqual(theranges.post_min, "Y")

    def test_leave_min(self):
        theranges.temp_min = 15.00
        theranges.temp_now = 20.00
        theranges.post_min = 'N'
        theranges.set_min

        self.assertEqual(theranges.temp_min, 15.00)
        self.assertEqual(theranges.post_min, "N")

    def test_raise_max(self):
        theranges.temp_max = 20.00
        theranges.temp_now = 25.00
        theranges.post_max = 'N'
        theranges.set_max

        self.assertEqual(theranges.temp_max, 25.00)
        self.assertEqual(theranges.post_max, 'Y')

    def test_leave_max(self):
        theranges.temp_max = 25.00
        theranges.temp_now = 20.00
        theranges.post_max = 'N'
        theranges.set_max

        self.assertEqual(theranges.temp_max, 25.00)
        self.assertEqual(theranges.post_max, 'N')

if __name__ == '__main__':
    unittest.main()
