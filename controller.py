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


def get_color_from_mode_state(mode, state):
    lower = trunc(state) % len(modes[mode])
    upper = ceil(state) % len(modes[mode])
    if upper == lower:
        upper = (upper + 1) % len(modes[mode])
    phase = state - trunc(state)
    return (int(round(modes[mode][lower][0] * (1 - phase) +
                      modes[mode][upper][0] * phase)),
            int(round(modes[mode][lower][1] * (1 - phase) +
                      modes[mode][upper][1] * phase)),
            int(round(modes[mode][lower][2] * (1 - phase) +
                      modes[mode][upper][2] * phase)))


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
            color = {
                'right': get_color_from_mode_state(mode, state),
                'top': get_color_from_mode_state(mode, state+0.5),
                'left': get_color_from_mode_state(mode, state+1),
                'bottom': get_color_from_mode_state(mode, state+1.5)
            }
            dmx_bus.set_channels({
                1: color['right'][0], 2: color['right'][1], 3: color['right'][2],
                6: color['top'][0], 7: color['top'][1], 8: color['top'][2],
                11: color['left'][0], 12: color['left'][1], 13: color['left'][2],
                16: color['bottom'][0], 17: color['bottom'][1], 18: color['bottom'][2]
            })
            state += 0.005
        if IO.input(12) and IO.input(13):
            dmx_bus.set_channels({1: 0, 2: 0, 3: 0})
        time.sleep(0.02)


if __name__ == "__main__":
    init()
    main_loop()
