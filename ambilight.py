#!/usr/bin/python3
import time
import picamera
import picamera.array



with picamera.PiCamera() as camera:
    while True:
        with picamera.array.PiRGBArray(camera) as stream:
            camera.resolution = (1024, 768)
            camera.capture(stream, 'rgb')
            print("Red:   " + str(stream.array[10, 180, 0]))
            print("Green: " + str(stream.array[10, 180, 1]))
            print("Blue:  " + str(stream.array[10, 180, 2]))
            time.sleep(0.02)
