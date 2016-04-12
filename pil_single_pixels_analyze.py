#!/usr/bin/python3
# -*- coding: utf-8 -*-
from __future__ import division, print_function, unicode_literals

from PIL import Image
from multiprocessing import Queue


def analyze(image, queue):
    right_pixel = image.getpixel((490, 240))
    top_pixel = image.getpixel((320, 50))
    left_pixel = image.getpixel((150, 240))
    queue.put({
        'left': left_pixel,
        'top': top_pixel,
        'right': right_pixel,
        'bottom': (0, 0, 0)
    })

if __name__ == "__main__":
    im = Image.open("test.jpg").convert('RGB')
    q = Queue()
    analyze(im, q)
    print(q.get())
