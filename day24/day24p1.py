import collections
from collections.abc import Callable
from dataclasses import dataclass
import operator
import sys


@dataclass
class Gate:
    input1: str
    input2: str
    op: Callable[[int, int], int]
    output: str


STR_TO_OP = {
    'OR': operator.__or__,
    'XOR': operator.xor,
    'AND': operator.__and__,
}


def read_initial_wire_values() -> dict[str, int]:
    wire_values = {}
    for line in sys.stdin:
        line = line.rstrip()
        if not line:
            break
        name, val = line.split(': ')
        wire_values[name] = int(val)
    return wire_values


def read_gates() -> collections.deque[Gate]:
    return collections.deque(
        Gate(
            input1=(p := line.split())[0], input2=p[2], op=STR_TO_OP[p[1]], output=p[-1]
        )
        for line in sys.stdin
    )


def collect_result(wire_values: dict[str, int], prefix: str) -> int:
    ns = [(name, val) for name, val in wire_values.items() if name.startswith(prefix)]
    ns.sort(reverse=True)
    result = 0
    for _, n in ns:
        result = (result << 1) | n
    return result


def main():
    wire_values = read_initial_wire_values()
    to_process = read_gates()

    # Could definitely order the gates better we didn't need to do multiple passes.
    while to_process:
        gate = to_process.popleft()
        if gate.input1 in wire_values and gate.input2 in wire_values:
            wire_values[gate.output] = gate.op(
                wire_values[gate.input1], wire_values[gate.input2]
            )
        else:
            to_process.append(gate)

    result = collect_result(wire_values, 'z')
    print(result)


if __name__ == '__main__':
    main()
