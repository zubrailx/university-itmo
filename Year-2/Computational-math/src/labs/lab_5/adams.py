from modules.util import ProjectException

def adams_solve(points, func, h, n):
    if (len(points) < 4):
        return points.copy()
    new_points = points[0:4].copy()
    for i in range(3, n - 1):
        xim3, xim2, xim1, xi = [p["x"] for p in new_points[(i - 3):(i + 1)]]
        yim3, yim2, yim1, yi = [p["y"] for p in new_points[(i - 3):(i + 1)]]
        yit = yi + h / 24 * (-9 * func(xim3, yim3) + 37 * func(xim2, yim2) -
                              59 * func(xim1, yim1) + 55 * func(xi, yi))
        yip1 = yi + h / 24 * (func(xim2, yim2) - 5 * func(xim1, yim1) + 19 * func(xi, yi) +
                              9 * func(xi + h, yit))
        new_points.append({"x": xi + h, "y": yip1})
    return new_points

