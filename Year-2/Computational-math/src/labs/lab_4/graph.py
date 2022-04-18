import matplotlib.pyplot as plt
import numpy as np

from labs.lab_4.spline import MultiSpline

# black - real points
# blue  - error points
# grey  - deleted point
# green - first spline
# turquoise - second spline
# red    - real equation
def draw(p_reals, p_errs, p_err_max_i, s_first: MultiSpline, s_second: MultiSpline, equation=None):
    xp = [p["x"] for p in p_reals]
    # display real points
    yp_real = [p["y"] for p in p_reals]
    real_colors = ["k" for _ in range(len(p_reals))]
    # display err points
    yp_errs = [p["y"] for p in p_errs]
    err_colors = ["b" for _ in range(len(p_errs))]
    err_colors[p_err_max_i] = '#9ea0a9'
    # display splines
    x_min = s_first.splines[0].c_point_0
    x_max = s_first.splines[-1].c_point_1
    x_step = (x_max - x_min) / 100
    x_npr = list(np.arange(x_min, x_max + x_step, x_step))
    y_ms1 = [s_first.calculate(x) for x in x_npr]
    y_ms2 = [s_second.calculate(x) for x in x_npr]
    # draw
    plt.scatter(xp, yp_real, c=real_colors, marker="o")
    plt.scatter(xp, yp_errs, c=err_colors, marker="o")
    plt.plot(x_npr, y_ms1, c="#156b42", label="first spline")
    plt.plot(x_npr, y_ms2, c="#80ffec", label="second spline")
    if (equation is not None):
        y_eq = [equation.calculate({"x": x}) for x in x_npr]
        plt.plot(x_npr, y_eq, c="#ff0101", label="real equation")
    plt.show()
