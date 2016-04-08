#!/usr/bin/python3
# -*- coding: utf-8 -*-
from __future__ import division, print_function, unicode_literals

from PIL import Image
import numpy as np
from matplotlib.path import Path


def get_pixel_count(path):
    count = 0
    for x in range(int(path.get_extents().xmin), int(path.get_extents().xmax)):
        for y in range(int(path.get_extents().ymin), int(path.get_extents().ymax)):
            if path.contains_point((x, y)):
                count += 1
    return count

polygons = {
    'left': Path([(11, 175), (11, 685), (111, 580), (111, 272)]),
    'top': Path([(36, 192), (140, 296), (771, 214), (931, 36)]),
    'right': Path([(931, 36), (771, 214), (788, 621), (931, 798)]),
    'bottom': Path([(11, 660), (933, 783), (788, 621), (111, 580)])
}
counts = dict()
for direction in polygons.keys():
    counts[direction] = get_pixel_count(polygons[direction])
print(counts)


def analyze(image):
    data = np.array(image)
    color_sums = dict()
    for key in polygons.keys():
        color_sums[key] = [0, 0, 0]
    for y in range(data.shape[0]):
        for x in range(data.shape[1]):
            for key in polygons.keys():
                if polygons[key].contains_point((x, y)):
                    color_sums[key][0] += data[y, x, 0]
                    color_sums[key][1] += data[y, x, 1]
                    color_sums[key][2] += data[y, x, 2]
    colors = dict()
    for key in polygons.keys():
        colors[key] = (int(round(color_sums[key][0]/counts[key])),
                       int(round(color_sums[key][1]/counts[key])),
                       int(round(color_sums[key][2]/counts[key])))
    return colors

if __name__ == "__main__":
    im = Image.open("test.jpg")
    print(analyze(im))
