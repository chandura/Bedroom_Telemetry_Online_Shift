import unittest

from clock_classes import stream_config
from clock_classes import ranges
from clock_functions import write_record
from clock_functions import post_record

from ISStreamer.Streamer import Streamer

configgroup = 'initialstate.test.com'

theranges = ranges(0.00, '00:00')
config = stream_config(configgroup)
streamer = Streamer(bucket_name=config.bucketname, bucket_key=config.bucketkey, access_key=config.accesskey)

class TestBetween(unittest.TestCase):

    def test_config(self):
        config = stream_config(configgroup)

        self.assertEqual(config.accesskey, 'jvrPfFpIg0LhMzBLkuuNkyhc4nbXGfSM')
        self.assertEqual(config.bucketkey, 'CCAQUFDTHUUW')
        self.assertEqual(config.bucketname, 'Stream Test')

    def test_record_write_1(self):
        write = write_record(1, streamer)
        self.assertEqual(write, 'N')

    def test_record_write_100(self):
        write = write_record(100, streamer)
        self.assertEqual(write, 'Y')

    def test_record_write_500(self):
        write = write_record(500, streamer)
        self.assertEqual(write, 'Y')

    def test_no_record_write_50(self):
        write = write_record(50, streamer)
        self.assertEqual(write, 'N')

    def test_no_record_write_701(self):
        write = write_record(701, streamer)
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
        theranges.time_now = '00:00'
        theranges.reset

        self.assertEqual(theranges.temp_min, 50.00)
        self.assertEqual(theranges.temp_max, 0.00)

    def test_not_reset_ranges(self):
        theranges.temp_min = 20.00
        theranges.temp_max = 20.00
        theranges.time_now = '00:01'
        theranges.reset
        self.assertEqual(theranges.temp_min, 20.00)
        self.assertEqual(theranges.temp_max, 20.00)

    #def test_lower_min(self):
        #max_min.min_temp = 25.00
        #max_min.min_temp = reset_max_min("00:01", max_min, 20.00)
        #self.assertEqual(max_min.min_temp, 20.00)

    #def test_leave_min(self):
        #max_min.min_temp = 15.00
        #min_temp = reset_max_min("11:23", max_min, 17.00)
        #self.assertEqual(min_temp, 15.00)

if __name__ == '__main__':
    unittest.main()