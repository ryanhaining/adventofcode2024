import dataclasses
from collections.abc import Callable
from typing import Optional


@dataclasses.dataclass
class State:
    reg_a: int = 0
    reg_b: int = 0
    reg_c: int = 0
    ic: int = 0
    output: list[int] = dataclasses.field(default_factory=lambda: [])

    def get_value(self, arg: int):
        match arg:
            case 4:
                return self.reg_a
            case 5:
                return self.reg_b
            case 6:
                return self.reg_c
        assert arg < 4
        return arg


class Computer:
    _state: State
    _op_to_fun: dict[int, Callable[[int], None]]

    def __init__(self, reg_a_init):
        self._state = State(reg_a=reg_a_init)
        self._op_to_fun = {
            0: self._adv,
            1: self._bxl,
            2: self._bst,
            3: self._jnz,
            4: self._bxc,
            5: self._out,
            6: self._bdv,
            7: self._cdv,
        }

    def run(self, program: list[int]) -> list[int]:
        while self._state.ic < len(program) - 1:
            self._dispatch(program[self._state.ic], program[self._state.ic + 1])
            self._state.ic += 2
        return self._state.output

    def _dispatch(self, op_code: int, arg: int) -> None:
        self._op_to_fun[op_code](arg)

    def _adv(self, arg: int) -> None:
        self._state.reg_a = self._state.reg_a // (1 << self._state.get_value(arg))

    def _bxl(self, literal: int) -> None:
        self._state.reg_b = self._state.reg_b ^ literal

    def _bst(self, arg: int) -> None:
        self._state.reg_b = self._state.get_value(arg) % 8

    def _jnz(self, literal: int) -> None:
        if self._state.reg_a != 0:
            self._state.ic = literal - 2  # -2 so the normal +2 can still happen

    def _bxc(self, _: int) -> None:
        self._state.reg_b = self._state.reg_b ^ self._state.reg_c

    def _out(self, arg: int) -> None:
        self._state.output.append(self._state.get_value(arg) % 8)

    def _bdv(self, arg: int) -> None:
        self._state.reg_b = self._state.reg_a // (1 << self._state.get_value(arg))

    def _cdv(self, arg: int) -> None:
        self._state.reg_c = self._state.reg_a // (1 << self._state.get_value(arg))


def run(program: list[int], reg_a_init: int) -> list[int]:
    computer = Computer(reg_a_init)
    return computer.run(program)


def solve(program: list[int], prefix: int, place: int) -> Optional[int]:
    if place == len(program):
        return prefix
    shift = (len(program) - 1 - place) * 3
    index = len(program) - (place + 1)
    for n in range(0b1000):
        current = prefix | (n << shift)
        output = run(program, current)
        if output[index] == program[index]:
            res = solve(program, current, place + 1)
            if res is not None:
                return res
    return None


def main() -> None:
    # skip reg values and blank line
    for _ in range(4):
        input()

    program = [int(n) for n in input().split()[-1].split(',')]

    start = 1 << (len(program) * 3 - 1)
    print(solve(program, prefix=start, place=1))


if __name__ == '__main__':
    main()
