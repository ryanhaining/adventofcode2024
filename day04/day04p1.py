import collections
import itertools
import sys

import collections
import itertools
import sys
from typing import Sequence, Iterable, TypeVar

T = TypeVar('T')


def sliding_window(seq: Iterable[T], size) -> Iterable[Sequence[T]]:
    it = iter(seq)
    elems = collections.deque(itertools.islice(it, size))
    yield elems
    for e in it:
        elems.popleft()
        elems.append(e)
        yield tuple(elems)


TERM = 'XMAS'
ANY_TERM = (TERM, TERM[::-1])
TERM_LEN = len(TERM)
LINE_PADDING = '.' * TERM_LEN

lines = sys.stdin.readlines()
# Pad the input so we can avoid bounds checking later.
lines = [f'{LINE_PADDING}{line.rstrip()}{LINE_PADDING}' for line in lines]

total = 0
for line in lines:
    total += sum(line.count(term) for term in ANY_TERM)

for chunk in sliding_window(lines, TERM_LEN):
    for col in range(TERM_LEN, len(line) - TERM_LEN):
        diag_fwd = ''.join(chunk[n][col + n] for n in range(TERM_LEN))
        diag_back = ''.join(chunk[n][col + (TERM_LEN - 1 - n)] for n in range(TERM_LEN))
        if diag_fwd in ANY_TERM:
            total += 1
        if diag_back in ANY_TERM:
            total += 1
        down = ''.join(chunk[row][col] for row in range(TERM_LEN))
        if down in ANY_TERM:
            total += 1


print(total)
