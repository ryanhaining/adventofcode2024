from dataclasses import dataclass
import itertools
import sys

HEIGHT = 103
WIDTH = 101
ROUNDS = 100


@dataclass
class Robot:
    position: tuple[int, int]
    velocity: tuple[int, int]


def parse_ints(piece: str) -> tuple[int, int]:
    ns = piece.split('=')[-1].split(',')
    assert len(ns) == 2
    return int(ns[1]), int(ns[0])


def parse_bot(line: str) -> Robot:
    parts = line.split()
    assert len(parts) == 2
    return Robot(parse_ints(parts[0]), parse_ints(parts[1]))


def flip(pos: tuple[int, int]) -> tuple[int, int]:
    return pos[0], WIDTH - (pos[1] + 1)


best = 0
bots = [parse_bot(line) for line in sys.stdin]
for i in itertools.count(1):
    for r in bots:
        r.position = (
            (r.position[0] + r.velocity[0]) % HEIGHT,
            (r.position[1] + r.velocity[1]) % WIDTH,
        )
    positions = {r.position for r in bots}  # { if r.position[1] > WIDTH//2}
    grid = [[' '] * WIDTH for _ in range(HEIGHT)]
    for r in bots:
        grid[r.position[0]][r.position[1]] = '#'
    cur = len(positions)
    if cur > best:
        best = cur
        print(i, best)
        for row in grid:
            print(''.join(str(e) for e in row))
        print('-' * WIDTH)
    if best == len(bots):
        print(i)
        break
