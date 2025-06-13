# This program is designed to create a red circle
# on the webcam display, and have a rover (blue circle)
# create an optimal path to the red circle.
# once the blue circle touches the red circle
# a new red circle appears, and repeat.

import cv2
import sys
import numpy as np
import matplotlib.pyplot as plt
import random as rand


def random_position():
    return (rand.randint(0,620), rand.randint(0,460))

s = 0
if len(sys.argv) > 1:
    s = sys.argv[1]

source = cv2.VideoCapture(s)
win_name = 'Camera Preview'
cv2.namedWindow(win_name, cv2.WINDOW_NORMAL)
(x, y, windowWidth, windowHeight) = cv2.getWindowImageRect(win_name)

rand_circle_pos = random_position()


while cv2.waitKey(1) != 27: # Escape
    has_frame, frame = source.read()
    if not has_frame:
        break

    frame_with_circle = frame.copy()

    cv2.circle(frame_with_circle, rand_circle_pos, 10, (255, 155, 0), 4)

    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    lower_red = np.array([0, 155, 140])
    upper_red = np.array([20, 255, 255])

    mask = cv2.inRange(hsv, lower_red, upper_red)

    result = cv2.bitwise_and(frame, frame, mask=mask)

    addition = cv2.add(frame_with_circle, result)

    cv2.imshow('with circle', frame_with_circle)
    cv2.imshow('mask', result)
    cv2.imshow(win_name, frame)
    cv2.imshow('merged', addition)

print("Origin Coordinates(x,y): ", x, y)
print("Width: ", windowWidth)
print("Height: ", windowHeight)

source.release()
cv2.destroyWindow(win_name)