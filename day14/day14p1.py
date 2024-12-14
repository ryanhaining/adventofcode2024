import collections
from dataclasses import dataclass
import enum
import math
from typing import Optional
import sys

HEIGHT = 103
WIDTH = 101
ROUNDS = 100


class Quadrant(enum.Enum):
    TOP_LEFT = enum.auto()
    TOP_RIGHT = enum.auto()
    BOTTOM_LEFT = enum.auto()
    BOTTOM_RIGHT = enum.auto()


def get_quad(pos: tuple[int, int]) -> Optional[Quadrant]:
    if pos[0] < HEIGHT // 2:
        if pos[1] < WIDTH // 2:
            return Quadrant.TOP_LEFT
        if pos[1] >= math.ceil(WIDTH / 2):
            return Quadrant.TOP_RIGHT
    elif pos[0] >= math.ceil(HEIGHT / 2):
        if pos[1] < WIDTH // 2:
            return Quadrant.BOTTOM_LEFT
        if pos[1] >= math.ceil(WIDTH / 2):
            return Quadrant.BOTTOM_RIGHT
    return None


@dataclass
class Robot:
    start_position: tuple[int, int]
    velocity: tuple[int, int]


def parse_ints(piece: str) -> tuple[int, int]:
    ns = piece.split('=')[-1].split(',')
    assert len(ns) == 2
    return int(ns[1]), int(ns[0])


def parse_bot(line: str) -> Robot:
    parts = line.split()
    assert len(parts) == 2
    return Robot(parse_ints(parts[0]), parse_ints(parts[1]))


counts: collections.Counter[tuple[int, int]] = collections.Counter()
for line in sys.stdin:
    r = parse_bot(line)
    end_pos = (
        ((r.start_position[0] + ROUNDS * r.velocity[0]) % HEIGHT),
        ((r.start_position[1] + ROUNDS * r.velocity[1]) % WIDTH),
    )
    counts[end_pos] += 1

quad_counts: collections.Counter[Quadrant] = collections.Counter()
for pos, c in counts.items():
    quad = get_quad(pos)
    if quad is not None:
        quad_counts[quad] += c

print(math.prod(quad_counts.values()))
