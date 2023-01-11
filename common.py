"""
Common functions & definitions
"""

from enum import Enum
from hashlib import sha1
import json
import os
from subprocess import PIPE, Popen
from sys import executable as PYTHON, platform
from typing import List, Tuple, Union

#############
# Functions #
#############

def get_file_sha1(path: str) -> bytes:
    """Gets the SHA1 hash of a file"""

    with open(path, 'rb') as f:
        return sha1(f.read()).digest()

def get_cmd_stdout(cmd: str, text=True) -> str:
    """Run a command and get the stdout output as a string"""

    proc = Popen(cmd.split(), stdout=PIPE, text=text)
    ret = proc.stdout.read()
    proc.wait()
    assert proc.returncode == 0, f"Command '{cmd}' returned {proc.returncode}"
    return ret

class Binary(Enum):
    DOL = 1
    REL = 2

# ppcdis source output
SourceDesc = Union[str, Tuple[str, int, int]]

def get_containing_slice(addr: int) -> Tuple[Binary, SourceDesc]:
    """Finds the binary containing an address and its source file
    Source file is empty string if not decompiled"""

    dol_raw = get_cmd_stdout(f"{SLICES} {DOL_YML} {DOL_SLICES} -p {DOL_SRCDIR}/ --containing {addr:x}")
    containing = json.loads(dol_raw)
    return (Binary.DOL, containing)

def find_headers(dirname: str, base=None) -> List[str]:
    """Returns a list of all headers in a folder recursively"""

    if base is None:
        base = dirname

    ret = []
    for name in os.listdir(dirname):
        path = dirname + '/' + name
        if os.path.isdir(path):
            ret.extend(find_headers(path, base))
        elif name.endswith('.h'):
            ret.append(path[len(base)+1:])

    return ret

################
# Project dirs #
################

# Directory for decompiled dol code
DOL_SRCDIR = "src"

# Include directory
INCDIR = "include"

# Build artifacts directory
BUILDDIR = "build"

# Build include directory
BUILD_INCDIR = f"{BUILDDIR}/include"

# Output binaries directory
OUTDIR = "out"

# Original binaries directory
ORIG = "orig"

# Tools directory
TOOLS = "tools"

# Config directory
CONFIG = "config"

#########
# Tools #
#########

# ppcdis
PPCDIS = "tools/ppcdis"
PPCDIS_INCDIR = f"{PPCDIS}/include"
ANALYSER = f"{PYTHON} {PPCDIS}/analyser.py"
DISASSEMBLER = f"{PYTHON} {PPCDIS}/disassembler.py"
ORDERSTRINGS = f"{PYTHON} {PPCDIS}/orderstrings.py"
ORDERFLOATS = f"{PYTHON} {PPCDIS}/orderfloats.py"
FORCEFILESGEN = f"{PYTHON} {PPCDIS}/forcefilesgen.py"
ELF2DOL = f"{PYTHON} {PPCDIS}/elf2dol.py"
SLICES = f"{PYTHON} {PPCDIS}/slices.py"

# Codewarrior
TOOLS = "tools"
CODEWARRIOR = os.path.join(TOOLS, "2.5")
CC = os.path.join(CODEWARRIOR, "mwcceppc")
LD = os.path.join(CODEWARRIOR, "mwldeppc")
if platform != "win32":
    CC = f"wine {CC}"
    LD = f"wine {LD}"

# DevkitPPC
DEVKITPPC = os.environ.get("DEVKITPPC")
AS = os.path.join(DEVKITPPC, "bin", "powerpc-eabi-as")
OBJDUMP = os.path.join(DEVKITPPC, "bin", "powerpc-eabi-objdump")

ICONV = f"{PYTHON} tools/sjis.py" # TODO: get actual iconv working(?)

#########
# Files #
#########

# Slices
DOL_SLICES = f"{CONFIG}/dol_slices.yml"

# Overrides (TODO: do these need to be separate for rel?)
ANALYSIS_OVERRIDES = f"{CONFIG}/analysis_overrides.yml"
DISASM_OVERRIDES = f"{CONFIG}/disasm_overrides.yml"

# Binaries
DOL = f"{ORIG}/main.dol" # read in python code
DOL_YML = f"{CONFIG}/dol.yml"
DOL_SHA = f"{ORIG}/main.dol.sha1"

# Symbols
SYMBOLS = f"{CONFIG}/symbols.yml"

# Analysis outputs
DOL_LABELS = f"{BUILDDIR}/labels.pickle"
DOL_RELOCS = f"{BUILDDIR}/relocs.pickle"

# Linker
DOL_LCF_TEMPLATE = f"{CONFIG}/dol.lcf"
DOL_LCF = f"{BUILDDIR}/dol.lcf"

# Outputs
DOL_ELF = f"{BUILDDIR}/main.elf"
DOL_OUT = f"{OUTDIR}/main.dol"
DOL_MAP = f"{OUTDIR}/main.map"

##############
# Tool Flags #
##############

ASFLAGS = ' '.join([
    "-m gekko",
    f"-I {INCDIR}",
    f"-I {PPCDIS_INCDIR}",
    f"-I orig"
])

CFLAGS = [
    "-lang=c++",
    "-fp fmadd",
    "-fp_contract on",
    "-Cpp_exceptions off",   
    "-rostr",
    "-RTTI off",
    "-char signed",
    "-enum int",
    "-use_lmw_stmw on"
]

JSYSTEM_BASE = CFLAGS + [ "-inline auto" ]
JSYSTEM = JSYSTEM_BASE + [ "-O4,p" ]
JUTILITY = JSYSTEM_BASE + [ "-O4,s" ]
BASE_GAME_CFLAGS = CFLAGS + [ "-O4" ]
BASE_DOL_CFLAGS = BASE_GAME_CFLAGS + [ "-inline on, auto" ]
OSAKO = BASE_DOL_CFLAGS + [ "-common off" ]
KANESHIGE = BASE_GAME_CFLAGS + [ "-inline off" ]

LOCAL_CFLAGS = [
    "-nostdinc",
    "-proc gekko",
    "-maxerrors 1",
    "-I-",
    f"-i {INCDIR}",
    f"-i {PPCDIS_INCDIR}",
    f"-i {BUILD_INCDIR}"
]
DOL_CFLAGS = ' '.join(BASE_DOL_CFLAGS + LOCAL_CFLAGS)
JUTILITY_CFLAGS = ' '.join(JUTILITY + LOCAL_CFLAGS)
JSYSTEM_CFLAGS = ' '.join(JSYSTEM + LOCAL_CFLAGS)
KANESHIGE_CFLAGS = ' '.join(KANESHIGE + LOCAL_CFLAGS)
OSAKO_CFLAGS = ' '.join(OSAKO + LOCAL_CFLAGS)
EXTERNAL_DOL_CFLAGS = ' '.join(BASE_DOL_CFLAGS)

LDFLAGS = ' '.join([
    "-fp hard",
    "-maxerrors 1",
    "-mapunused"
])

PPCDIS_ANALYSIS_FLAGS = ' '.join([
    f"-o {ANALYSIS_OVERRIDES}"
])

PPCDIS_DISASM_FLAGS = ' '.join([
    f"-m {SYMBOLS}",
    f"-o {DISASM_OVERRIDES}"
])

####################
# diff.py Expected #
####################

EXPECTED = "expected"
DOL_EXPECTED = f"{EXPECTED}/build/main.elf"