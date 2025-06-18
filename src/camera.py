# This program is a chaser program
# Designed to have a blue circle at a random location
# within the borders of the camera.
# And the program tracks a red circle on top of the rover
# such that, when the rover is near the blue circle
# the blue circle appears at a new randomized location.

import cv2
import sys
import numpy as np
import random as rand


def random_position():
    return (rand.randint(30,620), rand.randint(30,460))

s = 0
if len(sys.argv) > 1:
    s = sys.argv[1]

source = cv2.VideoCapture(s)
win_name = 'Camera Preview'
cv2.namedWindow(win_name, cv2.WINDOW_NORMAL)
(x, y, windowWidth, windowHeight) = cv2.getWindowImageRect(win_name)

rand_circle_pos = random_position()
rand_circle_radius = 10

def euclideanDistance(p1,p2):
    return np.linalg.norm(np.array(p1) - np.array(p2))

while cv2.waitKey(1) != 27: # Escape
    has_frame, frame = source.read()
    if not has_frame:
        break

    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    lower_red = np.array([0, 130, 120])
    upper_red = np.array([20, 255, 255])
    red_mask = cv2.inRange(hsv, lower_red, upper_red)
    
    red_mask = cv2.erode(red_mask, None, iterations=2)
    red_mask = cv2.dilate(red_mask, None, iterations=2)

    contours, _ = cv2.findContours(red_mask.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    robot_pos = None
    robot_radius = 0

    if contours:
        largest = max(contours, key=cv2.contourArea)
        ((x, y), radius) = cv2.minEnclosingCircle(largest)

        if radius > 10:
            robot_pos = (int(x), int(y))
            robot_radius = int(radius)

            cv2.circle(frame, robot_pos, robot_radius, (255, 0, 255), 3)
            cv2.circle(frame, robot_pos, 2, (0, 255, 255), -1)

    rand_circle = cv2.circle(frame, rand_circle_pos, rand_circle_radius, (255, 155, 0), 4)
    
    if robot_pos  is not None:
        distance = euclideanDistance(robot_pos, rand_circle_pos)
        if distance < robot_radius + rand_circle_radius:
            rand_circle_pos = random_position()

    cv2.imshow(win_name, frame)


print("Origin Coordinates(x,y): ", x, y)
print("Width: ", windowWidth)
print("Height: ", windowHeight)
print(robot_pos)

source.release()
cv2.destroyWindow(win_name)