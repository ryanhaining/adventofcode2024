import itertools
import collections
import sys


def in_bounds(pos: tuple[int, int], lines: list[str]) -> bool:
    return 0 <= pos[0] < len(lines) and 0 <= pos[1] < len(lines[0])


def add_all(
    lines: list[str],
    results: set[tuple[int, int]],
    init: tuple[int, int],
    diff: tuple[int, int],
) -> None:
    cur = (init[0] + diff[0], init[1] + diff[1])
    del init
    while in_bounds(cur, lines):
        result.add(cur)
        cur = (cur[0] + diff[0], cur[1] + diff[1])


lines = [line.rstrip() for line in sys.stdin]

locations: collections.defaultdict[str, list[tuple[int, int]]] = (
    collections.defaultdict(list)
)

for row_index, row in enumerate(lines):
    for col_index, col in enumerate(row):
        if col != '.':
            locations[col].append((row_index, col_index))

result = set(itertools.chain.from_iterable(locations.values()))
for locs in locations.values():
    for i1 in range(len(locs)):
        for i2 in range(i1 + 1, len(locs)):
            r1, c1 = locs[i1]
            r2, c2 = locs[i2]

            row_diff = abs(r1 - r2)
            col_diff = abs(c1 - c2)
            if r1 < r2 and c1 < c2:
                add_all(
                    lines, result, (min(r1, r2), min(c1, c2)), (-row_diff, -col_diff)
                )
                add_all(lines, result, (max(r1, r2), max(c1, c2)), (row_diff, col_diff))
            else:
                add_all(
                    lines, result, (min(r1, r2), max(c1, c2)), (-row_diff, col_diff)
                )
                add_all(
                    lines, result, (max(r1, r2), min(c1, c2)), (row_diff, -col_diff)
                )

print(len(result))
