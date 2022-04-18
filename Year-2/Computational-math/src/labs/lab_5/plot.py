import matplotlib.pyplot as plt
import numpy as np
from labs.lab_4.spline import MultiSpline


def draw(points, func: MultiSpline, real_func=None):
    new_points = sorted(points, key=lambda p: p["x"])
    x = [p["x"] for p in new_points]
    y = [p["y"] for p in new_points]
    color_p = ["b" for _ in range(len(points))]
    plt.scatter(x, y, c=color_p, marker="o")

    x_min = new_points[0]["x"]
    x_max = new_points[-1]["x"]
    x_step = (x_max - x_min) / len(points)
    x_draw = list(np.arange(x_min, x_max + x_step, x_step))
    y_draw = [func.calculate(x) for x in x_draw]
    plt.plot(x_draw, y_draw, c="#156b42")
    
    if (real_func is not None):
        yr_draw = [real_func.calculate({"x": x}) for x in x_draw]
        plt.plot(x_draw, yr_draw, c="#ff0101", label="real_equation")
    plt.show()
