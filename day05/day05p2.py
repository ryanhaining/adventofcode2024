import collections
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
    to_count = False
    # We can ignore the second half since we only need the middle value
    for i in range(len(update) // 2 + 1):
        again = True
        while again:
            again = False
            for j in range(i + 1, len(update)):
                n = update[j]
                cur = update[i]
                if cur in order[n]:
                    update.pop(i)
                    update.insert(update.index(n) + 1, cur)
                    to_count = True
                    again = True
    if to_count:
        total += update[len(update) // 2]


print(total)
