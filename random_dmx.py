#!/usr/bin/python3
# -*- coding: utf-8 -*-
from __future__ import division, print_function, unicode_literals
from dmx import DmxBus
from random import randint


def random_colors(dmx_bus):
    while True:
        dmx_bus.set_channels({
            1: randint(0, 255), 2: randint(0, 255), 3: randint(0, 255)
        })

if __name__ == "__main__":
    bus = DmxBus()
    random_colors(bus)
