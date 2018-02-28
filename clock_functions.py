import json
from datetime import datetime

def write_record (inputcount, loops, streamer):

    write_next = 'N'
    if (inputcount % loops) == 0:
        #print("Write to Dashboard %s" % (inputcount/100000))
        print("Input count %d" % (inputcount/loops))
        #streamer.log("Input Count", (inputcount/100000))
        write_next = 'Y'

    #Discard the first reading from the serial input is it seems to be badly formed
    # #print("Test for the first reading")
    if (inputcount) == 1:
        write_next = 'N'

    return write_next

def post_record(data, write_next, sensor_output):

    if data == "{" and write_next == "Y":
        data = json.loads(sensor_output)
        #print("JSON Found")
        value_to_post = 'Y'
        #write_next = 'N'
        print("Data %s" % data)
        return (value_to_post, data)
    else:
        #print("No JSON found")
        value_to_post = 'N'
        return (value_to_post, 'not applicable')

def post_value(key, value, streamer, post_count, theranges):

    #print("Post Count %s" % post_count)
    theranges.time_now = datetime.now().strftime('%H:%M')
    #print("The time now is %s" % theranges.time_now)
    theranges.reset
    #post_count = post_count + 1
    if key == "Temperature":
        theranges.temp_now = value
        theranges.set_min
        theranges.set_max
        streamer.log("Input Count", post_count)
        streamer.log("Temperature_Now", value)
        if theranges.post_min == "Y":
            streamer.log("MinTemp", theranges.temp_min)
            theranges.post_min = "N"
        if theranges.post_max == "Y":
            streamer.log("MaxTemp", theranges.temp_max)
            theranges.post_max = "N"