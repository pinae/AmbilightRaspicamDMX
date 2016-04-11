#!/usr/bin/python3
# -*- coding: utf-8 -*-
from __future__ import division, print_function, unicode_literals
import RPi.GPIO as IO
import time
import ambilight
from dmx import DmxBus
from math import trunc, ceil
ambilight_is_running = False
lava = 0
water = 1
spectrum = 2
modes = {
    lava: [(255, 0, 0), (255, 210, 0)],
    water: [(0, 0, 255), (0, 255, 234)],
    spectrum: [(0, 255, 0), (0, 128, 128), (0, 0, 255), (128, 0, 128), (255, 0, 0), (128, 128, 0)]
}
dmx_bus = DmxBus()


def init():
    IO.setmode(IO.BOARD)
    IO.setup(12, IO.IN)
    IO.setup(13, IO.IN)
    IO.setup(15, IO.IN)
    IO.setup(16, IO.IN)


def start_ambilight():
    if not ambilight_is_running:
        ambilight.start(dmx_bus)
        print("Ambilight started.")


def stop_ambilight():
    if ambilight_is_running:
        ambilight.shutdown()
        print("Ambilight stopped.")


def main_loop():
    mode = spectrum
    state = 0.0
    while True:
        if not IO.input(12) and IO.input(13):
            start_ambilight()
        else:
            stop_ambilight()
        if IO.input(12) and not IO.input(13):
            if IO.input(15) and not IO.input(16) and mode != lava:
                mode = lava
                state = 0.0
            if IO.input(15) and IO.input(16) and mode != water:
                mode = water
                state = 0.0
            if not IO.input(15) and IO.input(16) and mode != spectrum:
                mode = spectrum
                state = 0.0
            color = (modes[mode][trunc(state) % len(modes[mode])][0] * (state - trunc(state)) +
                     modes[mode][ceil(state) % len(modes[mode])][0] * (1 - (state - trunc(state))),
                     modes[mode][trunc(state) % len(modes[mode])][1] * (state - trunc(state)) +
                     modes[mode][ceil(state) % len(modes[mode])][1] * (1 - (state - trunc(state))),
                     modes[mode][trunc(state) % len(modes[mode])][2] * (state - trunc(state)) +
                     modes[mode][ceil(state) % len(modes[mode])][2] * (1 - (state - trunc(state))))
            dmx_bus.set_channels({1: color[0], 2: color[1], 3: color[2]})
            state += 0.01
            print(str(mode) + ": " + str(state) + " ... color: " + str(color))
        time.sleep(0.05)


if __name__ == "__main__":
    init()
    main_loop()
