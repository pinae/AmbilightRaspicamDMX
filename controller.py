#!/usr/bin/python3
# -*- coding: utf-8 -*-
from __future__ import division, print_function, unicode_literals
import RPi.GPIO as IO
import time


def init():
    IO.setmode(IO.BOARD)
    IO.setup(12, IO.IN)
    IO.setup(13, IO.IN)
    IO.setup(15, IO.IN)
    IO.setup(16, IO.IN)


def main_loop():
    while True:
        print([IO.input(12), IO.input(13), IO.input(15), IO.input(16)])
        time.sleep(0.05)


if __name__ == "__main__":
    init()
    main_loop()
