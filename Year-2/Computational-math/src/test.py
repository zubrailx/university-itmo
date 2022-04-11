import labs.lab_4.graph as graph
import modules.parse as parse

p_reals = [{"x": -1, "y": -0.841}, {"x": 0, "y": 0}, {"x": 1, "y": 0.841}, {"x": 2, "y": 0.909}]
p_errs = [{"x": -1, "y": -0.741}, {"x": 0, "y": 0.1}, {"x": 1, "y": 0.861}, {"x": 2, "y": 0.949}]
p_err_max_i = 1
s_first = []
s_second = []
equation = parse.parse_expression("\sin x")


graph.draw(p_reals, p_errs, p_err_max_i, s_first, s_second, equation)
