#!/usr/bin/python3
# -*- coding: utf-8 -*-
from __future__ import division, print_function, unicode_literals
import os


class DmxBus(object):
    def __init__(self):
        self.channels = [0] * 255

    def set_channels(self, channels_dict):
        for channel_number in channels_dict.keys():
            if type(channel_number) is int and type(channels_dict[channel_number]) is int:
                self.channels[channel_number] = channels_dict[channel_number]
        self.write()

    def write(self):
        value_string = ",".join([str(x) for x in self.channels])
        os.system("ola_set_dmx -u 0 -d " + value_string)
