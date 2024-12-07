import operator
import sys
from typing import Iterable


def has_solution(ans: int, so_far: int, nums: list[int]) -> bool:
    if not nums:
        return ans == so_far
    n, *rest = nums
    return has_solution(ans, so_far * n, rest) or has_solution(ans, so_far + n, rest)


def line_val(line: str) -> int:
    ans, first, *nums = [int(n) for n in line.split()]
    return ans if has_solution(ans, first, nums) else 0


def format_lines(lines: Iterable[str]) -> Iterable[str]:
    return (line.replace(':', '').rstrip() for line in lines)


print(sum(line_val(line) for line in format_lines(sys.stdin)))
