import cv2 as cv
import numpy as np
import json
from datetime import datetime


def read_colors_from_file(path):
    file = open(path, "r")
    data = json.loads(" ".join(file.readlines()))
    data_min, data_max = [d["min"] for d in data], [d["max"] for d in data]
    return (data_min, data_max)


def draw_rectangles(frame, n, colors=None, thickness=2):
    (height, width, channels) = frame.shape
    if (channels == 4):
        # alpha blue green red
        col = [255, 0, 0, 255]
    elif (channels == 3):
        col = [255, 0, 0]
    else:
        raise Exception("Image should have at least 3 dimensions")

    if (colors is not None):
        if (len(colors) < n):
            raise Exception("len(colors of rectangles) should be at least {}".format(n))
    else:
        colors = [col for _ in range(n)]

    rw, rh = (width - thickness * (n)) / n, height / n
    ws = rw
    rectangle_dims = []
    for i in range(n):
        top_left = (int(ws * i) + thickness * i, int((height - rh) / 2))
        bottom_right = (int(ws * (i + 1)) + thickness * i, int((height + rh) / 2))
        cv.rectangle(frame, top_left, bottom_right, colors[i], thickness)
        rectangle_dims.append((top_left, bottom_right))
    return rectangle_dims


def get_masked_frame_from_rectangles(frame, rectangles):
    frames = []
    for i in range(len(rectangles)):
        hsv_frame = cv.cvtColor(frame, cv.COLOR_BGR2HSV)
        top_left, bottom_right = rectangles[i][0], rectangles[i][1]
        mask_frame = hsv_frame[top_left[1]:bottom_right[1] + 1, top_left[0]:bottom_right[0] + 1]
        frames.append(mask_frame)
    return frames


def check_masked_frames(masked_frames, colors_min, colors_max, coef=0.9):
    if (len(colors_max) != len(colors_min) or len(masked_frames) != len(colors_min)):
        raise Exception("Invalid arguments")
    rect_rez = []
    for i in range(len(masked_frames)):
        lower = np.array(colors_min[i])
        upper = np.array(colors_max[i])
        mask_colors = cv.inRange(masked_frames[i], lower, upper)
        c = np.count_nonzero(mask_colors)/mask_colors.size
        rect_rez.append((int(c > coef), c))
    return rect_rez


history = "never"

def put_text_with_stroke(frame, text, org, font, font_scale, color, thickness, sc):
    cv.putText(frame, text, org, font, font_scale, sc, thickness + 1)
    cv.putText(frame, text, org, font, font_scale, color, thickness)

def display_results(frame, results, coordinates):
    org = (10, frame.shape[0] - 10)
    font = cv.FONT_HERSHEY_TRIPLEX
    font_scale = 0.5
    text_color = (255, 255, 255)
    stroke_color = (0, 0, 0)
    thickness = 1
    line_break = 25
    for i in range(len(results)):
        text = f"rect[{i}] = {results[i][0] == 1}, coef = {round(results[i][1], 4)}"
        put_text_with_stroke(frame, text, (org[0], org[1] - (len(results) - 1 - i) * line_break),
                             font, font_scale, text_color, thickness, stroke_color)

    org = (frame.shape[1] - 280, frame.shape[0] - 10)
    if (sum([r[0] for r in results]) == len(results)):
        now = datetime.now()
        global history
        history = now.strftime("%H:%M:%S:%f")
    text = "Last time unlocked: " + history
    put_text_with_stroke(frame, text, org, font, font_scale, text_color, thickness, stroke_color)


def main():
    cap = cv.VideoCapture(0)
    if (not cap.isOpened()):
        print("Cannot open camera.")
        exit()

    (colors_min, colors_max) = read_colors_from_file("colors.json")
    colors_med = [[int((colors_min[i][j] + colors_max[i][j]) / 2)
                   for j in range(len(colors_min[i]))] for i in range(len(colors_min))]
    n = len(colors_med)
    while True:
        # reading frame
        ret, frame = cap.read()
        if not ret:
            print("Can't receive frame (stream end?). Exiting ...")
            break
        # inverting
        frame = cv.flip(frame, 180)
        # process
        rectangle_dims = draw_rectangles(frame, n, thickness=2, colors=colors_med)
        masked_frames = get_masked_frame_from_rectangles(frame, rectangle_dims)
        rect_rez = check_masked_frames(masked_frames, colors_min=colors_min, colors_max=colors_max, coef=0.9)
        display_results(frame, rect_rez, rectangle_dims)
        # display + process heystroke
        cv.imshow('frame', frame)
        if (cv.waitKey(1) & 0xFF) == ord('q'):
            break
    cap.release()
    cv.destroyAllWindows()


if __name__ == '__main__':
    main()
