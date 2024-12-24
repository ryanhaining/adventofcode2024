import collections
import sys


def all_connected(
    connections: dict[str, set[str]], clique: set[str], candidate: str
) -> bool:
    return len(connections[candidate] & clique) == len(clique)


def main():
    connections: collections.defaultdict[str, set[str]] = collections.defaultdict(set)

    for line in sys.stdin:
        line = line.rstrip()
        c1, c2 = line.split('-')
        connections[c1].add(c2)
        connections[c2].add(c1)

    max_clique: tuple[str, ...] = tuple()
    for src, dests in connections.items():
        clique = {src}
        for d in dests:
            if all_connected(connections, clique, d):
                clique.add(d)
        if len(max_clique) < len(clique):
            max_clique = tuple(clique)

    print(','.join(sorted(max_clique)))


if __name__ == '__main__':
    main()
