import collections
import itertools
import sys

order = collections.defaultdict(set)

for line in sys.stdin:
    line = line.rstrip()
    if not line:
        break
    n1, n2 = line.split('|')

    order[int(n2)].add(int(n1))

total = 0
for line in sys.stdin:
    update = [int(n) for n in line.rstrip().split(',')]
    rest = set(update)
    for cur in update:
        rest.remove(cur)
        if rest & order[cur]:
            break
    else:
        total += update[len(update) // 2]

print(total)
