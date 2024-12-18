"""
Bunch of globals. Don't be like me.

"""

reg_a = 0
reg_b = 0
reg_c = 0
ic = 0
output = []


def get_value(arg: int):
    match arg:
        case 4:
            return reg_a
        case 5:
            return reg_b
        case 6:
            return reg_c
    assert arg < 4
    return arg


def adv(arg: int):
    global reg_a
    reg_a = reg_a // (1 << get_value(arg))


def bxl(literal: int):
    global reg_b
    reg_b = reg_b ^ literal


def bst(arg: int):
    global reg_b
    reg_b = get_value(arg) % 8


def jnz(literal: int):
    global ic
    if reg_a != 0:
        ic = literal - 2  # -2 so the +2 can still happen


def bxc(_: int):
    global reg_b
    reg_b = reg_b ^ reg_c


def out(arg: int):
    output.append(get_value(arg) % 8)


def bdv(arg: int):
    global reg_b
    reg_b = reg_a // (1 << get_value(arg))


def cdv(arg: int):
    global reg_c
    reg_c = reg_a // (1 << get_value(arg))


OP_TO_FUN = {
    0: adv,
    1: bxl,
    2: bst,
    3: jnz,
    4: bxc,
    5: out,
    6: bdv,
    7: cdv,
}


def read_reg_init() -> int:
    return int(input().split(': ')[-1])


reg_a = read_reg_init()
reg_b = read_reg_init()
reg_c = read_reg_init()

input()  # skip blank

ic = 0

program = [int(n) for n in input().split()[-1].split(',')]

while ic < len(program) - 1:
    OP_TO_FUN[program[ic]](program[ic + 1])
    ic += 2

print(*output, sep=',')
