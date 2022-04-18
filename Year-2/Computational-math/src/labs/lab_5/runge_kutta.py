def runge_kutta_solve(x0, y0, func, h, n):
    points = []
    x_cur = x0
    y_cur = y0
    points.append({"x": x_cur, "y": y_cur})
    for _ in range(1, n):
        ki1 = func(x_cur, y_cur)
        ki2 = func(x_cur + h / 2, y_cur + h / 2 * ki1)
        ki3 = func(x_cur + h / 2, y_cur + h / 2 * ki2)
        ki4 = func(x_cur + h, y_cur + h * ki3)
        dyi = h / 6 * (ki1 + 2 * ki2 + 2 * ki3 + ki4)
        x_cur += h
        y_cur += dyi
        points.append({"x": x_cur, "y": y_cur})
    return points


