import itertools
import sys


def weight(n: int, rules: set[tuple[int, int]]) -> int:
    return sum(r[0] == n for r in rules)


all_rules: set[tuple[int, int]] = {
    (int((ns := line.split('|'))[0]), int(ns[1]))
    for line in itertools.takewhile(lambda s: s != '\n', sys.stdin)
}


total = 0
for line in sys.stdin:
    update = [int(n) for n in line.rstrip().split(',')]
    relevant = {r for r in all_rules if r[0] in update and r[1] in update}
    correct = sorted(update, key=lambda n: weight(n, relevant), reverse=True)
    if correct != update:
        total += correct[len(correct) // 2]


print(total)
