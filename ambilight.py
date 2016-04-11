#!/usr/bin/python3
# -*- coding: utf-8 -*-
from __future__ import division, print_function, unicode_literals

import io
import time
import threading
import picamera
from PIL import Image
from dmx import DmxBus
from analyze_image import analyze
from multiprocessing.queues import Empty

# Create a pool of image processors
done = False
lock = threading.Lock()
pool = []
dmx_bus = None


class ImageProcessor(threading.Thread):
    def __init__(self, queue):
        super(ImageProcessor, self).__init__()
        self.queue = queue
        self.stream = io.BytesIO()
        self.event = threading.Event()
        self.terminated = False
        self.start()

    def run(self):
        # This method runs in a separate thread
        global done
        while not self.terminated:
            # Wait for an image to be written to the stream
            if self.event.wait(1):
                try:
                    self.stream.seek(0)
                    # Read the image and do some processing on it
                    image = Image.open(self.stream).convert('RGB')
                    color = analyze(image)
                    if dmx_bus:
                        dmx_bus.set_channels({
                            1: color['right'][0], 2: color['right'][0], 3: color['right'][0],
                            6: color['top'][0], 7: color['top'][0], 8: color['top'][0],
                            11: color['left'][0], 12: color['left'][0], 13: color['left'][0],
                            16: color['bottom'][0], 17: color['bottom'][0], 18: color['bottom'][0]
                        })
                    # Terminate if True is in the queue
                    if not self.queue.empty() and self.queue.get(block=False):
                        done = True
                except Empty:
                    done = False
                finally:
                    # Reset the stream and event
                    self.stream.seek(0)
                    self.stream.truncate()
                    self.event.clear()
                    # Return ourselves to the pool
                    with lock:
                        pool.append(self)


def streams():
    while not done:
        with lock:
            if pool:
                processor = pool.pop()
            else:
                processor = None
        if processor:
            yield processor.stream
            processor.event.set()
        else:
            # When the pool is starved, wait a while for it to refill
            time.sleep(0.1)


# Shut down the processors in an orderly fashion
def shutdown():
    while pool:
        with lock:
            processor = pool.pop()
        processor.terminated = True
        processor.join()


def start(assigned_dmx_bus, queue):
    with picamera.PiCamera() as camera:
        global pool
        global dmx_bus
        dmx_bus = assigned_dmx_bus
        pool += [ImageProcessor(queue) for _ in range(4)]
        camera.resolution = (640, 480)
        camera.framerate = 30
        camera.start_preview()
        time.sleep(1.2)
        camera.shutter_speed = camera.exposure_speed
        camera.exposure_mode = 'off'
        g = camera.awb_gains
        camera.awb_mode = 'off'
        camera.awb_gains = g
        camera.capture_sequence(streams(), use_video_port=True)


if __name__ == "__main__":
    start(DmxBus())
    shutdown()
