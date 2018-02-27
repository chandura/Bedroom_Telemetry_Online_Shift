#! /usr/bin/env python

import serial
import os
import sys
from datetime import datetime

from clock_classes import stream_config, ranges
from clock_functions import write_record, post_record, post_value

from ISStreamer.Streamer import Streamer

inputcount = 1
write_next = 'N'
post_count = 0
min_temp = 0.00

if (os.name=='nt'):
    running_on = "test"
    configgroup = "initialstate.test.com"
    loops = 10
elif (os.name=='posix'):
    running_on = "live"
    configgroup = "initialstate.com"
    loops = 100
else:
    running_on = "unknown"
    configgroup = "unknown"
    sys.exit()

config = stream_config(configgroup)
theranges = ranges(0.00, '00:00')
streamer = Streamer(bucket_name=config.bucketname, bucket_key=config.bucketkey, access_key=config.accesskey)

print("Started at %s" % datetime.now().strftime('%d-%m-%Y %H:%M'))
print("The access key is %s" % config.accesskey)
print("The bucket key is %s" % config.bucketkey)
print("The bucket to use is %s" % config.bucketname)
print("The delay loop is set to %s" % loops)

streamer.log("My Messages", "Waiting for temp")

if running_on == 'test':
    port = "COM5"
    ser = serial.Serial(port, 38400)
else:
    ser = serial.Serial('/dev/ttyUSB0', 9600)

print("Running on %s and monitoring on %s" % (running_on, ser.name))

while True:
    #streamer.log("My Messages", "Value Aquired")
    if write_next == 'N':
        write_next = write_record(inputcount, loops, streamer)
        if running_on == 'test':
            print("Write next %s.  Input count %d" % (write_next, inputcount))

    sensor_output = str(ser.readline().strip())
    #print("Sensor output %s" % sensor_output)

    if (write_next) == 'Y':
        #sensor_output = str(ser.readline().strip())
        print("Sensor Output %s " % sensor_output)
        if running_on == 'test':
            sensor_output = sensor_output.strip('b\'')
        else:
            sensor_output = sensor_output.strip('b\'')
        print("Checking %s" % sensor_output[0])
        data = sensor_output[0]
        print("Data tested, found %s write_next is %s " % (data, write_next))
        value_to_post, data = post_record(data, write_next, sensor_output)
        print("Value to post %s and data is %s " % (value_to_post, data))

        if (value_to_post) == 'Y':
            inputcount = 0
            write_next = 'N'
            post_count = post_count + 1
            for key, value in data.items():
                print("Key %s and value %s (post count %s)" % (key, value, post_count))
                max_min = post_value(key, value, streamer, post_count, theranges)

    inputcount = inputcount + 1
    streamer.close()