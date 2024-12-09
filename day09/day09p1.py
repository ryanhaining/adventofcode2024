import sys
from typing import Optional


nums = [int(n) for n in sys.stdin.readlines()[0].rstrip()]

counter = 0

fs: list[Optional[int]] = []
free_flag = False
for n in nums:
    if free_flag:
        fs.extend([None] * n)
    else:
        fs.extend([counter] * n)
        counter += 1
    free_flag = not free_flag

low = 0
high = len(fs) - 1
while low < high:
    while fs[low] is not None:
        low += 1
    while fs[high] is None:
        high -= 1
    if low < high:
        fs[low], fs[high] = fs[high], fs[low]

print(sum(i * e for i, e in enumerate(fs) if e is not None))
