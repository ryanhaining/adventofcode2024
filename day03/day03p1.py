import re
import sys

print(
    sum(
        int(n1) * int(n2)
        for n1, n2 in re.findall(r'mul\((\d+),(\d+)\)', sys.stdin.read())
    )
)
