from dataclasses import dataclass
import re
import sys
from typing import Iterable


@dataclass
class Machine:
    ax: int
    ay: int
    bx: int
    by: int
    x_goal: int
    y_goal: int


def cramer(m: Machine) -> tuple[float, float]:
    """Cramer's Rule

    https://en.wikipedia.org/wiki/Cramer%27s_rule#Explicit_formulas_for_small_systems
    """
    a_pushes = (m.x_goal * m.by - m.bx * m.y_goal) / (m.ax * m.by - m.bx * m.ay)
    b_pushes = (m.ax * m.y_goal - m.x_goal * m.ay) / (m.ax * m.by - m.bx * m.ay)
    return a_pushes, b_pushes


def parse_nums(line: str) -> tuple[int, int]:
    if not line:
        raise ValueError('empty line')
    nums = re.findall('(\d+)', line)
    assert len(nums) == 2
    return int(nums[0]), int(nums[1])


def machines(goal_offset: int = 0) -> Iterable[Machine]:
    while True:
        try:
            ax, ay = parse_nums(sys.stdin.readline())
        except ValueError:
            break
        bx, by = parse_nums(sys.stdin.readline())
        x_goal, y_goal = parse_nums(sys.stdin.readline())
        sys.stdin.readline()  # skip blank
        yield Machine(ax, ay, bx, by, x_goal + goal_offset, y_goal + goal_offset)


def is_int(n: float) -> bool:
    return n % 1 < 0.0000001


def cost_or_zero(a_pushes: float, b_pushes: float) -> int:
    return (
        int(a_pushes) * 3 + int(b_pushes)
        if is_int(a_pushes) and is_int(b_pushes)
        else 0
    )


if __name__ == '__main__':
    print(sum(cost_or_zero(*cramer(m)) for m in machines()))
