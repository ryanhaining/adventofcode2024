import collections
import heapq
from dataclasses import dataclass
import sys
from typing import Optional


MAX_SIZE = 9


@dataclass
class Block:
    index: int
    size: int
    f_id: int


nums = [int(n) for n in sys.stdin.readlines()[0].rstrip()]

# block size -> heap  of indices
free_blocks: collections.defaultdict[int, list[int]] = collections.defaultdict(list)

counter = 0
fs: list[Block] = []
free_flag = False
index = 0
for n in nums:
    if free_flag:
        if n:
            heapq.heappush(free_blocks[n], index)
    else:
        fs.append(Block(index=index, size=n, f_id=counter))
        counter += 1
    index += n
    free_flag = not free_flag


def find_first_free(size: int, free_blocks: dict[int, list[int]]) -> Optional[int]:
    # https://github.com/python/mypy/issues/14664 for type: ignore
    # mypy thinks the key function neds to be able to handle None
    return min(
        (sz for sz in range(size, MAX_SIZE + 1) if free_blocks[sz]),
        key=lambda sz: free_blocks[sz][0],  # type: ignore
        default=None,
    )


for block in reversed(fs):
    free_block_size = find_first_free(block.size, free_blocks)
    if free_block_size is None:
        continue
    free_block_index = free_blocks[free_block_size][0]
    if free_block_index > block.index:
        continue
    block.index = free_block_index
    new_free_size = free_block_size - block.size
    new_free_index = free_block_index + block.size
    heapq.heappop(free_blocks[free_block_size])
    if new_free_size:
        heapq.heappush(free_blocks[new_free_size], new_free_index)

print(sum(sum(b.f_id * n for n in range(b.index, b.index + b.size)) for b in fs))
