"""
Disassembles a single function
"""

from argparse import ArgumentParser
from os import system, unlink
import os.path
from tempfile import NamedTemporaryFile

import common as c


def get_function(binary: c.Binary, source: c.SourceDesc, addr: int, extra: bool, inline=False) -> str:
    # Get context
    ctx = c.DOL_CTX if binary == c.Binary.DOL else c.REL_CTX
    assert os.path.exists(ctx.labels), "Error: analysis has not ran!"

    # Disassemble function
    extraflag = "-e" if extra else ""
    srcflag = f"-n {source}" if isinstance(source, str) else ""
    inlineflag = "-i" if inline else ""
    with NamedTemporaryFile(suffix=".s", delete=False) as tmp:
        try:
            tmp.close()
            ret = system(
                f"{c.DISASSEMBLER} {ctx.binary} {ctx.labels} {ctx.relocs} {tmp.name} -f {addr:x} "
                f"-m {c.SYMBOLS} {srcflag} -q {extraflag} {inlineflag}"
            )
            assert ret == 0, f"Disassembly error code {ret}"
            with open(tmp.name) as f:
                asm = f.read()
        finally:
            unlink(tmp.name)

    return asm


if __name__ == "__main__":
    parser = ArgumentParser()
    parser.add_argument("sym", type=str, help="Symbol name or address")
    parser.add_argument("-e", "--extra", action="store_true",
                        help="Include referenced jumptables")
    parser.add_argument("-d", "--dol", action="store_true",
                        help="Prioritise dol-local symbols")
    parser.add_argument("-r", "--rel", action="store_true",
                        help="Prioritise rel-local symbols")
    parser.add_argument("-n", "--source-name", type=str,
                        help="Prioritise source-local symbols")
    parser.add_argument("-i", "--inline", action="store_true",
                        help="Output as inline assembly")
    args = parser.parse_args()

    # Find address
    assert not (args.dol and args.rel), "--dol and --rel are incompatible"
    addr = c.lookup_sym(args.sym, args.dol, args.rel, args.source_name)
    assert addr is not None, f"Symbol {args.sym} not found"

    # Find containing binary
    binary, source = c.get_containing_slice(addr)

    print(get_function(binary, source, addr, args.extra, args.inline))
