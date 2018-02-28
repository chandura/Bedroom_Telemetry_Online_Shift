import configparser
from ISStreamer.Streamer import Streamer

configParser = configparser.RawConfigParser()
configFilePath = r'./clockstream.ini'
configParser.read(configFilePath)

class stream_config:

    def __init__(self, configgroup):
        self.configgroup = configgroup
        self.accesskey
        self.bucketkey
        self.bucketname

    @property
    def accesskey(self):
        #print(self.configgroup)
        return configParser.get(self.configgroup, 'accesskey')

    @property
    def bucketkey(self):
        return configParser.get(self.configgroup, 'bucketkey')

    @property
    def bucketname(self):
        return configParser.get(self.configgroup, 'bucketname')

class ranges:

    def __init__(self, temp_now, time_now):
        self.temp_now = temp_now
        self.time_now = time_now
        self.temp_min = 50.00
        self.post_min = 'N'
        self.temp_max = 0.00
        self.post_max = 'N'

    @property
    def reset(self):
        if self.time_now == '00:00':
            self.temp_min = 50.00
            self.temp_max = 0.00
        else:
            self.temp_min = self.temp_min
            self.temp_max = self.temp_max

    @property
    def set_min(self):
        if float(self.temp_now) < float(self.temp_min):
            self.temp_min = self.temp_now
            self.post_min = 'Y'

    @property
    def set_max(self):
        if float(self.temp_now) > float(self.temp_max):
            self.temp_max = self.temp_now
            self.post_max = 'Y'
