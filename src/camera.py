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

prevCircle = None
dist = lambda x1, y1, x2, y2: (x1-x2)**2+(y1-y2)**2
while cv2.waitKey(1) != 27: # Escape
    has_frame, frame = source.read()
    if not has_frame:
        break

    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    lower_red = np.array([0, 130, 120])
    upper_red = np.array([20, 255, 255])
    mask = cv2.inRange(hsv, lower_red, upper_red)
    mask = cv2.erode(mask, None, iterations=2)
    mask = cv2.dilate(mask, None, iterations=2)

    contours, _ = cv2.findContours(mask.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    robot_pos = None
    if contours:
        largest = max(contours, key=cv2.contourArea)
        ((x, y), radius) = cv2.minEnclosingCircle(largest)

        if radius > 10:
            center = (int(x), int(y))
            radius = int(radius)
            robot_pos = center

            cv2.circle(frame, center, radius, (255, 0, 255), 3)
            cv2.circle(frame, center, 2, (0, 255, 255), -1)


    masked = cv2.bitwise_and(frame, frame, mask=mask)
    grayFrame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    blurFrame = cv2.GaussianBlur(grayFrame, (17,17), 0)
    ret, thresh1 = cv2.threshold(mask, 150, 255, cv2.THRESH_BINARY)

    circles = cv2.HoughCircles(blurFrame, cv2.HOUGH_GRADIENT, 1.2, 50, 
                               param1=100, param2=30, minRadius=10, maxRadius=200)

    if circles is not None:
        circles = np.uint16(np.around(circles))
        chosen = None
        for i in circles[0, :]:
            if chosen is None: chosen = i
            if prevCircle is not None:
                if dist(chosen[0], chosen[1], prevCircle[0], prevCircle[1]) <= dist(i[0], i[1], prevCircle[0], prevCircle[1]):
                    chosen = i
        cv2.circle(frame, (chosen[0], chosen[1]), 1, (0,100,100), 3)
        cv2.circle(frame, (chosen[0], chosen[1]), chosen[2], (255, 0, 255), 3)
        prevCircle = chosen

    cv2.imshow("circles", frame)
    cv2.imshow("something", thresh1)
    frame_with_circle = frame.copy()

    circle = cv2.circle(frame_with_circle, rand_circle_pos, 10, (255, 155, 0), 4)

    addition = cv2.add(frame_with_circle, masked)
    
    #cv2.imshow('with circle', frame_with_circle)
    #cv2.imshow('mask', masked)
    #cv2.imshow(win_name, frame)
    cv2.imshow('merged', addition)

print("Origin Coordinates(x,y): ", x, y)
print("Width: ", windowWidth)
print("Height: ", windowHeight)

source.release()
cv2.destroyWindow("circles")