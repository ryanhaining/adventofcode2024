import collections
import sys


lines = [line.rstrip() for line in sys.stdin]


d = collections.defaultdict(list)

for row_index, row in enumerate(lines):
    for col_index, col in enumerate(row):
        if col != '.':
            d[col].append((row_index, col_index))

result = set()
for locs in d.values():
    for i1 in range(len(locs)):
        for i2 in range(i1 + 1, len(locs)):
            r1, c1 = locs[i1]
            r2, c2 = locs[i2]

            row_diff = abs(r1 - r2)
            col_diff = abs(c1 - c2)
            if r1 < r2 and c1 < c2:
                result.add((min(r1, r2) - row_diff, min(c1, c2) - col_diff))
                result.add((max(r1, r2) + row_diff, max(c1, c2) + col_diff))
            else:
                result.add((min(r1, r2) - row_diff, max(c1, c2) + col_diff))
                result.add((max(r1, r2) + row_diff, min(c1, c2) - col_diff))


result = {
    loc for loc in result if 0 <= loc[0] < len(lines) and 0 <= loc[1] < len(lines[0])
}
print(len(result))
