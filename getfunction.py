"""
Disassembles a single function
"""

from argparse import ArgumentParser
from os import system, unlink
from tempfile import NamedTemporaryFile

import common as c

def get_function(binary: c.Binary, srcflag: str, addr: int) -> str:
    # Get flags for binary
    if binary == c.Binary.DOL:
        binary = c.DOL_YML
        binflags = ""
        anlflags = f"{c.DOL_LABELS} {c.DOL_RELOCS}"
    else:
        binary = c.REL_YML
        binflags = c.PPCDIS_REL_FLAGS
        anlflags = f"{c.REL_LABELS} {c.REL_RELOCS}"

    # Disassemble function
    with NamedTemporaryFile(suffix=".c", delete=False) as tmp:
        try:
            tmp.close()
            ret = system(
                f"{c.DISASSEMBLER} {binary} {anlflags} {tmp.name} -f {addr:x} "
                f"-m {c.SYMBOLS} {binflags} {srcflag} -q"
            )
            assert ret == 0, f"Disassembly error code {ret}"
            with open(tmp.name) as f:
                asm = f.read()
        finally:
            unlink(tmp.name)
    
    return asm

if __name__=="__main__":
    parser = ArgumentParser()
    hex_int = lambda s: int(s, 16)
    parser.add_argument("addr", type=hex_int)
    args = parser.parse_args()

    # Find containing binary
    binary, source = c.get_containing_slice(args.addr)

    # Get source file name flag
    srcflag = f"-n {source}" if isinstance(source, str) else ""

    print(get_function(binary, srcflag, args.addr))