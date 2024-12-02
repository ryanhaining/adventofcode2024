import itertools
import sys


def in_range(n1: int, n2: int) -> bool:
    return 0 < abs(n1 - n2) < 4


def is_safe(nums: list[int]) -> bool:
    is_ascending = nums[1] > nums[0]
    return all(
        in_range(n1, n2) and (n2 > n1) == is_ascending
        for n1, n2 in itertools.pairwise(nums)
    )


print(sum(is_safe([int(n) for n in line.split()]) for line in sys.stdin))
