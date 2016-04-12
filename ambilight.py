#!/usr/bin/python3
# -*- coding: utf-8 -*-
from __future__ import division, print_function, unicode_literals

import io
import time
import threading
import picamera
from PIL import Image
from dmx import DmxBus
from pil_single_pixels_analyze import analyze
from multiprocessing.queues import Empty
from multiprocessing import Process, Queue

# Create a pool of image processors
done = False
lock = threading.Lock()
pool = []
dmx_bus = None


def analyze_worker(image_queue, result_queue):
    while True:
        image = image_queue.get()
        analyze(image, result_queue)


class ImageProcessor(threading.Thread):
    def __init__(self, queue):
        super(ImageProcessor, self).__init__()
        self.image_queue = Queue()
        self.result_queue = Queue()
        self.analyzer_process = Process(target=analyze_worker, args=(self.image_queue, self.result_queue))
        self.analyzer_process.start()
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
                    self.image_queue.put(image)
                    if not self.result_queue.empty():
                        color = self.result_queue.get()
                        if dmx_bus:
                            dmx_bus.set_channels({
                                1: int(round(color['right'][0])),
                                2: int(round(color['right'][1])),
                                3: int(round(color['right'][2])),
                                6: int(round(color['top'][0])),
                                7: int(round(color['top'][1])),
                                8: int(round(color['top'][2])),
                                11: int(round(color['left'][0])),
                                12: int(round(color['left'][1])),
                                13: int(round(color['left'][2])),
                                16: int(round(color['bottom'][0])),
                                17: int(round(color['bottom'][1])),
                                18: int(round(color['bottom'][2]))
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

    def shutdown(self):
        self.analyzer_process.join()


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
        processor.shutdown()
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
    start(DmxBus(), Queue())
    shutdown()
