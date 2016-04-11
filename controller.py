#!/usr/bin/python3
# -*- coding: utf-8 -*-
from __future__ import division, print_function, unicode_literals
import RPi.GPIO as IO
import time
import ambilight
from dmx import DmxBus
from math import trunc, ceil
from multiprocessing import Process, Queue
ambilight_process = None
ambilight_queue = Queue()
lava = 0
water = 1
spectrum = 2
modes = {
    lava: [(255, 0, 0), (128, 128, 0), (208, 48, 0), (138, 118, 0)],
    water: [(0, 0, 255), (0, 128, 128), (10, 30, 215), (0, 148, 108)],
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
    global ambilight_process
    global ambilight_queue
    if not ambilight_process:
        ambilight_process = Process(target=ambilight.start, args=(dmx_bus, ambilight_queue))
        ambilight_process.start()
        print("Ambilight started.")


def stop_ambilight():
    global ambilight_process
    global ambilight_queue
    if ambilight_process:
        ambilight_queue.put(True)
        ambilight_process.join(500)
        ambilight_process = None
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
            lower = trunc(state) % len(modes[mode])
            upper = ceil(state) % len(modes[mode])
            if upper == lower:
                upper = (upper+1) % len(modes[mode])
            phase = state - trunc(state)
            color = (modes[mode][lower][0] * (1 - phase) +
                     modes[mode][upper][0] * phase,
                     modes[mode][lower][1] * (1 - phase) +
                     modes[mode][upper][1] * phase,
                     modes[mode][lower][2] * (1 - phase) +
                     modes[mode][upper][2] * phase)
            dmx_bus.set_channels({1: round(color[0]), 2: round(color[1]), 3: round(color[2])})
            state += 0.01
        if IO.input(12) and IO.input(13):
            dmx_bus.set_channels({1: 0, 2: 0, 3: 0})
        time.sleep(0.05)


if __name__ == "__main__":
    init()
    main_loop()
