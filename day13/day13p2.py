OFFSET = 10_000_000_000_000

from day13p1 import cramer, cost_or_zero, machines

if __name__ == '__main__':
    print(sum(cost_or_zero(*cramer(m)) for m in machines(OFFSET)))
