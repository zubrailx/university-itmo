import sys
import json

from . import methods

method_dict = {
    "split_half" :      methods.split_half,
    "tangent":          methods.tangent,
    "simple_iteration": methods.simple_iteration
}

def solve(stream_input, stream_output):
    method_list = json.loads(" ".join(line.strip() for line in stream_input.readlines()))
    for m in method_list:
        if not (m["method"] in method_dict.keys()):
            continue
        # can be parsed
        result = method_dict[m["method"]](m)
        stream_output.write(json.dumps(result))
    return
