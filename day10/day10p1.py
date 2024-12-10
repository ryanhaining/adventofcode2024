import itertools
import sys

START = 0
GOAL = 9
BAD = -1


def bfs(board: list[list[int]], cur_row: int, cur_col: int) -> set[tuple[int, int]]:
    cur_val = board[cur_row][cur_col]
    if cur_val == GOAL:
        return set([(cur_row, cur_col)])
    opts = [
        t
        for t in (
            (cur_row - 1, cur_col),
            (cur_row + 1, cur_col),
            (cur_row, cur_col - 1),
            (cur_row, cur_col + 1),
        )
        if board[t[0]][t[1]] == cur_val + 1
    ]
    reachable = set(
        itertools.chain.from_iterable(bfs(board, opt[0], opt[1]) for opt in opts)
    )
    if not reachable:
        board[cur_row][cur_col] = BAD
    return reachable


# add border
board: list[list[int]] = [
    [BAD] + [int(n) for n in line.rstrip()] + [BAD] for line in sys.stdin
]
padding = [BAD] * len(board[0])
board = [padding] + board + [padding]

starts: set[tuple[int, int]] = set(
    (row_index, col_index)
    for row_index, row in enumerate(board)
    for col_index, col in enumerate(row)
    if col == START
)

print(sum(len(bfs(board, st[0], st[1])) for st in starts))
