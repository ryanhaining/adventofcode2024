import sys

left = []
right = []

for line in sys.stdin:
    n1, n2 = line.split()
    left.append(int(n1))
    right.append(int(n2))

left.sort()
right.sort()
print(sum(abs(n1 - n2) for n1, n2 in zip(left, right)))
