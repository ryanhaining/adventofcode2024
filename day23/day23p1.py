import collections
import sys


connections: collections.defaultdict[str, set[str]] = collections.defaultdict(set)

for line in sys.stdin:
    line = line.rstrip()
    c1, c2 = line.split('-')
    connections[c1].add(c2)
    connections[c2].add(c1)

groups: set[tuple[str, str, str]] = set()

for src, dests in connections.items():
    for d1 in dests:
        for d2 in dests:
            if d1 != d2:
                if d1 in connections[d2] and any(
                    c.startswith('t') for c in (src, d1, d2)
                ):
                    key = tuple(sorted((src, d1, d2)))
                    # mypy can't tell how long the tuple is
                    groups.add(key)  # type: ignore

print(len(groups))
