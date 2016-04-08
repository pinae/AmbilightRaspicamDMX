#!/usr/bin/python3
# -*- coding: utf-8 -*-
from __future__ import division, print_function, unicode_literals

from PIL import Image
import numpy as np


# The mask image is based on a grey image with 100% red for the left part of the image, 100% green for the top,
# 100% blue for the right and 0% green for the bottom. It must have the dimensions of the analyzed image.
mask_image = np.array(Image.open("mask.png"))
masks = {
    'left': mask_image[:, :, 0] > 160,
    'top': mask_image[:, :, 1] > 160,
    'right': mask_image[:, :, 2] > 160,
    'bottom': mask_image[:, :, 1] < 160
}


def analyze(image):
    data = np.array(image)
    averages = dict()
    for key in masks.keys():
        averages[key] = np.average(data[masks[key]], axis=0)
    return averages

if __name__ == "__main__":
    im = Image.open("test.jpg")
    print(analyze(im))
