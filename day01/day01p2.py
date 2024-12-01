import collections
import sys

counts = collections.Counter()
nums = []

for line in sys.stdin:
    n1, n2 = line.split()
    nums.append(int(n1))
    counts[int(n2)] += 1

print(sum(n * counts[n] for n in nums))
