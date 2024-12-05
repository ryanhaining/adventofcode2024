import collections
import itertools
import sys

order = collections.defaultdict(list)

for line in sys.stdin:
    line = line.rstrip()
    if not line:
        break
    n1, n2 = line.split('|')

    order[int(n1)].append(int(n2))

total = 0
for line in sys.stdin:
    update = [int(n) for n in line.rstrip().split(',')]
    for i, cur in enumerate(update):
        if any(cur in order[n] for n in itertools.islice(update, i + 1, None)):
            break
    else:
        total += update[len(update) // 2]

print(total)
