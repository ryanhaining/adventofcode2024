from dataclasses import dataclass
import sys
from typing import Optional


@dataclass
class Block:
    free: bool
    size: int
    f_id: Optional[int]


nums = [int(n) for n in sys.stdin.readlines()[0].rstrip()]

counter = 0
fs: list[Block] = []
free_flag = False
for n in nums:
    if free_flag:
        fs.append(Block(free=True, size=n, f_id=None))
    else:
        fs.append(Block(free=False, size=n, f_id=counter))
        counter += 1
    free_flag = not free_flag


def find_first_free(size: int, blocks: list[Block]) -> Optional[int]:
    for i, b in enumerate(blocks):
        if b.free and b.size >= size:
            return i
    return None


def find_block(f_id: int, blocks: list[Block]) -> int:
    for i, b in enumerate(blocks):
        if b.f_id == f_id:
            return i
    raise ValueError('not found')


cur_id = counter
while cur_id > 0:
    cur_id -= 1
    block_index = find_block(cur_id, fs)
    block = fs[block_index]
    free_index = find_first_free(block.size, fs)
    if free_index is None:
        continue
    free_block = fs[free_index]
    if free_index < block_index:
        fs[block_index] = Block(free=True, size=block.size, f_id=None)
        fs.insert(free_index, block)
        free_block.size -= block.size

index = 0
total = 0
for b in fs:
    if not b.free:
        assert b.f_id is not None
        total += sum(b.f_id * n for n in range(index, index + b.size))
    index += b.size
print(total)
