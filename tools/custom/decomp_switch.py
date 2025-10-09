from dataclasses import dataclass
from sys import argv

# made by kiwi

class BranchInst:
    NONE = 0
    EQ = 1
    NOT_EQ = 2
    GREATER = 3
    LESS = 4
    GREATER_OR_EQ = 5
    LESS_OR_EQ = 6
    ALWAYS = 7


BRANCH_OPCODE_TO_ENUM = {
    "beq": BranchInst.EQ,
    "bne": BranchInst.NOT_EQ,
    "bgt": BranchInst.GREATER,
    "blt": BranchInst.LESS,
    "bge": BranchInst.GREATER_OR_EQ,
    "ble": BranchInst.LESS_OR_EQ,
    "b": BranchInst.ALWAYS
}


class CompareInst:
    NONE = 0
    IMM = 1
    IMM_LOGICAL = 2


class CR:
    LT = (1 << 0)
    GT = (1 << 1)
    EQ = (1 << 2)


@dataclass
class Branch:
    inst: BranchInst
    dest: str


@dataclass
class Flow:
    addr: int
    inst: CompareInst
    imm: int
    branches: list[Branch]


@dataclass
class Case:
    branch: BranchInst
    dest: str


@dataclass
class Result:
    inst: BranchInst
    value: int

    def __repr__(self) -> str:
        rep = str(self.value)
        if self.inst == BranchInst.EQ:
            rep += "*"
        return rep


def raw_input() -> str:
    print("Paste your disassembled code (doldisasm format):")
    contents = []
    while True:
        try:
            line = input()
            if line == "":
                break
        except EOFError:
            break
        contents.append(line)
    return contents


def trim_inst(inst):
    return inst[inst.find("*/") + 2:]


def parse_imm(imm) -> int:
    imm = imm.casefold()
    if imm.startswith("0x"):
        return int(imm[2:], 16)
    return int(imm)


def flow_group_get_node(group, addr) -> int:
    for i in range(len(group)):
        if group[i].addr == addr:
            return i
    return -1


def build_flow(code) -> list[Flow]:
    flow_data = list()

    assert code[0].startswith("/*"), "Assembly is not in doldisasm format!"
    assert len(code[0]) > 37, "Assembly is not in doldisasm format!"

    tokens = trim_inst(code[0]).split()
    assert len(tokens) != 0, "Assembly is not in doldisasm format!"
    assert tokens[0].startswith(
        "cmp"), "Assembly does not begin with compare instruction!"

    branches = list()
    cmp = CompareInst.NONE
    imm = 0
    addr = 0
    for line in code:
        if not line.startswith("/*"):
            continue

        tokens = trim_inst(line).split()

        # Build flow
        if tokens[0].startswith("cmp"):
            # Existing flow
            if cmp != CompareInst.NONE:
                flow_data.append(Flow(addr, cmp, imm, branches.copy()))
                cmp = CompareInst.NONE
                imm = 0
                branches.clear()
            if tokens[0] == "cmplwi":
                addr = int(line.split()[1], 16)
                imm = parse_imm(tokens[2]) + 2**32
                cmp = CompareInst.IMM_LOGICAL
            elif tokens[0] == "cmpwi":
                addr = int(line.split()[1], 16)
                imm = parse_imm(tokens[2])
                cmp = CompareInst.IMM
            else:
                assert False, "Non-imm comparison not supported"
        # Build branches
        elif tokens[0].startswith("b"):
            opcode = tokens[0].replace("-", "").replace("+", "")
            if opcode in BRANCH_OPCODE_TO_ENUM:
                tokens[1] = tokens[1].replace(".L_", "lbl_")
                branches.append(
                    Branch(BRANCH_OPCODE_TO_ENUM[opcode], tokens[1]))
            else:
                assert False, f"Non supported branch instruction: {opcode}"

    # Append last flow
    flow_data.append(Flow(addr, cmp, imm, branches.copy()))
    return flow_data


def traverse_flow(flow_group, value) -> Case:
    # Root flow node
    flow_idx = 0
    flow = flow_group[flow_idx]

    # Traverse flow
    while True:
        # Set up CR
        cr = 0
        if value < flow.imm:
            cr |= CR.LT
        if value == flow.imm:
            cr |= CR.EQ
        if value > flow.imm:
            cr |= CR.GT
        assert (cr & (CR.GT | CR.LT)) != (CR.GT | CR.LT), "Illegal CR"

        # Take branch
        dst = -1
        b = BranchInst.NONE
        for branch in flow.branches:
            if branch.inst == BranchInst.ALWAYS:
                dst = int(branch.dest[4:], 16)
                b = branch.inst
                break
            elif branch.inst == BranchInst.EQ and (cr & CR.EQ):
                dst = int(branch.dest[4:], 16)
                b = branch.inst
                break
            elif branch.inst == BranchInst.NOT_EQ and (cr & CR.EQ) == 0:
                dst = int(branch.dest[4:], 16)
                b = branch.inst
                break
            elif branch.inst == BranchInst.GREATER and (cr & CR.GT):
                dst = int(branch.dest[4:], 16)
                b = branch.inst
                break
            elif branch.inst == BranchInst.LESS and (cr & CR.LT):
                dst = int(branch.dest[4:], 16)
                b = branch.inst
                break
            elif branch.inst == BranchInst.GREATER_OR_EQ and ((cr & CR.GT) or (cr & CR.EQ)):
                dst = int(branch.dest[4:], 16)
                b = branch.inst
                break
            elif branch.inst == BranchInst.LESS_OR_EQ and ((cr & CR.LT) or (cr & CR.EQ)):
                dst = int(branch.dest[4:], 16)
                b = branch.inst
                break

        # Did we take a branch?
        if dst != -1:
            # Check if branch exits switch
            flow_idx = flow_group_get_node(flow_group, dst)
            if flow_idx != -1:
                flow = flow_group[flow_idx]
            else:
                flow = None
        # Proceed through function
        else:
            flow_idx += 1
            flow = flow_group[flow_idx]
        # Did we exit the switch (branch to case label)?
        if flow == None:
            # Switch case label
            return Case(b, f"lbl_{hex(dst)[2:].upper()}")


def custom_key(case):
    return int(case[4:], 16)


def show_help():
    print("========================")
    print("decomp_switch.py")
    print("========================")
    print("Usage:")
    print("1. decomp_switch.py my_asm.s")
    print("    - Run with path to assembly file")
    print("2. decomp_switch.py")
    print("    - Run, then paste contents of assembly file")
    print("[IMPORTANT]")
    print("- Assembly MUST be in doldisasm format")
    print("- Assembly must begin at switch (compare instruction), and must end after")
    print("  the last instruction of the binary search comparisons.")


def main():
    code_str = ""
    if len(argv) < 2:
        code_str = raw_input()
    else:
        if argv[1].casefold() == "-h":
            show_help()
            return

        with open(argv[1], "r") as f:
            code_str = f.readlines()

    flow = build_flow(code_str)
    min_iter = int(input("Min value (inclusive) for switch statement:"))
    max_iter = int(input("Max value (inclusive) for switch statement:")) + 1

    # Dictionary format
    # cases = dict[str, Result]
    cases = dict()

    for i in range(min_iter, max_iter):
        case = traverse_flow(flow, i)
        if not case.dest in cases:
            cases[case.dest] = list()
        cases[case.dest].append(Result(case.branch, i))

    print("====================================================")
    print("Switch analysis:")
    print("====================================================")
    print("(Labels are sorted by their order in the asm,")
    print("so they should be in the original order.)")
    print("====================================================")
    print("Asterisk symbol (*) represents BEQ cases,")
    print("or cases guaranteed to be explicitly specified.")
    print("====================================================")
    print("This can help you pick out explicit cases from the default cases.")
    print("====================================================")

    cases_list = [case for case in cases]
    for case in sorted(cases_list, key=custom_key):
        print(f"Label: {case}")
        values = [str(x) for x in cases[case]]
        print(f"    - Cases: {', '.join(values)}")


main()
