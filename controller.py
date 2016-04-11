#!/usr/bin/python3
# -*- coding: utf-8 -*-
from __future__ import division, print_function, unicode_literals
import RPi.GPIO as IO
import time
lava = 0
water = 1
spectrum = 2


def init():
    IO.setmode(IO.BOARD)
    IO.setup(12, IO.IN)
    IO.setup(13, IO.IN)
    IO.setup(15, IO.IN)
    IO.setup(16, IO.IN)


def start_ambilight():
    print("Amilight started.")


def stop_ambilight():
    print("Ambilight stopped.")


def main_loop():
    while True:
        if not IO.input(12) and IO.input(13):
            start_ambilight()
        else:
            stop_ambilight()
        if IO.input(12) and not IO.input(13):
            mode = spectrum
            if IO.input(15) and not IO.input(16):
                mode = lava
            if IO.input(15) and IO.input(16):
                mode = water
            print(mode)
        time.sleep(0.05)


if __name__ == "__main__":
    init()
    main_loop()
