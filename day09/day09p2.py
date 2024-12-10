import collections
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

# block size -> list of indices
free_blocks: collections.defaultdict[int, list[int]] = collections.defaultdict(list)

counter = 0
fs: list[Block] = []
free_flag = False
index = 0
for n in nums:
    if free_flag:
        if n:
            free_blocks[n].append(index)
    else:
        fs.append(Block(index=index, size=n, f_id=counter))
        counter += 1
    index += n
    free_flag = not free_flag


def find_first_free(
    size: int, free_blocks: dict[int, list[int]]
) -> Optional[tuple[int, int]]:
    options = []
    for s in range(size, MAX_SIZE + 1):
        if free_blocks[s]:
            options.append((s, min(free_blocks[s])))
    return min(options, key=lambda t: t[1]) if options else None


for block in reversed(fs):
    find_result = find_first_free(block.size, free_blocks)
    if find_result is None:
        continue
    free_block_size, free_block_index = find_result
    if free_block_index > block.index:
        continue
    block.index = free_block_index
    free_blocks[free_block_size].remove(free_block_index)
    new_free_size = free_block_size - block.size
    new_free_index = free_block_index + block.size
    if new_free_size:
        free_blocks[new_free_size].append(new_free_index)

total = 0
for b in fs:
    total += sum(b.f_id * n for n in range(b.index, b.index + b.size))
print(total)
