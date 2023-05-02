"""
Common functions & definitions
"""

from dataclasses import dataclass
from enum import Enum
from hashlib import sha1
import json
import os
from subprocess import PIPE, run
from sys import executable as PYTHON, platform
from typing import List, Tuple, Union

import yaml
try:
    from yaml import CLoader as Loader
except ImportError:
    from yaml import Loader

#############
# Functions #
#############

def get_file_sha1(path: str) -> bytes:
    """Gets the SHA1 hash of a file"""

    with open(path, 'rb') as f:
        return sha1(f.read()).digest()

def get_cmd_stdout(cmd: str, text=True) -> str:
    """Run a command and get the stdout output as a string"""

    ret = run(cmd.split(), stdout=PIPE, text=text)
    assert ret.returncode == 0, f"Command '{cmd}' returned {ret.returncode}"
    return ret.stdout

class Binary(Enum):
    DOL = 1
    REL = 2

# ppcdis source output
SourceDesc = Union[str, Tuple[str, int, int]]


def get_containing_slice(addr: int) -> Tuple[Binary, SourceDesc]:
    """Finds the binary containing an address and its source file
    Source file is empty string if not decompiled"""

    dol_raw = get_cmd_stdout(
        f"{SLICES} {DOL_YML} {DOL_SLICES} --containing {addr:x}")
    containing = json.loads(dol_raw)
    return (Binary.DOL, containing)


def lookup_sym(sym: str, dol: bool = False, rel: bool = False, source_name: str = None) -> int:
    """Takes a symbol as a name or address and returns the address"""

    # Get binary
    if dol:
        binary_name = DOL_YML
    else:
        binary_name = None

    # Determine type
    try:
        return int(sym, 16)
    except ValueError:
        return get_address(sym, binary_name, source_name)


def lookup_sym_full(sym: str, dol: bool = False, rel: bool = False, source_name: str = None
                    ) -> int:
    """Takes a symbol as a name or address and returns both the name and address"""

    # Get binary
    if dol:
        binary_name = DOL_YML
    else:
        binary_name = None

    # Determine type
    try:
        return int(sym, 16), get_name(sym)
    except ValueError:
        return get_address(sym, binary_name, source_name), sym


def get_address(name: str, binary: bool = None, source_name: bool = None) -> int:
    """Finds the address of a symbol"""

    args = [name]
    if binary is not None:
        args.append(f"-b {binary}")
    if source_name is not None:
        args.append(f"-n {source_name}")

    raw = get_cmd_stdout(
        f"{SYMBOLSCRIPT} {SYMBOLS} --get-addr {' '.join(args)}")
    return json.loads(raw)


def get_name(addr: int, binary: bool = None, source_name: bool = None) -> int:
    """Finds the name of a symbol"""

    args = [addr]
    if binary is not None:
        args.append(f"-b {binary}")
    if source_name is not None:
        args.append(f"-n {source_name}")

    raw = get_cmd_stdout(
        f"{SYMBOLSCRIPT} {SYMBOLS} --get-name {' '.join(args)}")
    return json.loads(raw)

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


def load_from_yaml(path: str, default=None):
    """Loads an object from a yaml file"""

    if default is None:
        default = {}
    with open(path) as f:
        ret = yaml.load(f.read(), Loader)
        if ret is None:
            ret = default
        return ret


##################
# Target options #
##################

BUILD_YML = load_from_yaml("config/build_opts.yml")
REGION = BUILD_YML['region']
VERSION = BUILD_YML['version']

################
# Project dirs #
################

# Directory for decompiled dol code
DOL_SRCDIR = "src"

# Include directory
INCDIR = "include"

# Tools directory
TOOLS = "tools"

# Main config directory
MAIN_CONFIG = "config"

# subdir
VERSION_DIR = f"{VERSION}_{REGION}"

# Build artifacts directory
BUILDDIR = f"build/{VERSION_DIR}"

# Build include directory
BUILD_INCDIR = f"{BUILDDIR}/include"

# Output binaries directory
OUTDIR = f"out/{VERSION_DIR}"

# Original binaries directory
ORIG = f"orig/{VERSION_DIR}"

# Version specififc config directory
CONFIG = f"config/{VERSION_DIR}"

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
ASSETRIP = f"{PYTHON} {PPCDIS}/assetrip.py"
ASSETINC = f"{PYTHON} {PPCDIS}/assetinc.py"
FORCEFILESGEN = f"{PYTHON} {PPCDIS}/forcefilesgen.py"
ELF2DOL = f"{PYTHON} {PPCDIS}/elf2dol.py"
FORCEACTIVEGEN = f"{PYTHON} {PPCDIS}/forceactivegen.py"
SLICES = f"{PYTHON} {PPCDIS}/slices.py"
PROGRESS = f"{PYTHON} {PPCDIS}/progress.py"
SYMBOLSCRIPT = f"{PYTHON} {PPCDIS}/symbols.py"

# Codewarrior
SDK_CW = os.path.join(TOOLS, "1.2.5")
SDK_CC = os.path.join(SDK_CW, "mwcceppc")

MW_1_3_2 = os.path.join(TOOLS, "1.3.2")
CC_1_3_2 = os.path.join(MW_1_3_2, "mwcceppc")

CODEWARRIOR = os.path.join(TOOLS, "2.5")
CC = os.path.join(CODEWARRIOR, "mwcceppc")
LD = os.path.join(CODEWARRIOR, "mwldeppc")
if platform != "win32": # perhaps make a way to figure out if wibo exists, if not, default to wine
    SDK_CC = f"wine {SDK_CC}"
    CC = f"wine {CC}"
    LD = f"wine {LD}"

# DevkitPPC
DEVKITPPC = os.environ.get("DEVKITPPC")
if DEVKITPPC is None:
    DEVKITPPC = os.path.join(TOOLS, "devkitppc")
    assert(os.path.isdir(DEVKITPPC))

AS = os.path.join(DEVKITPPC, "bin", "powerpc-eabi-as")
OBJDUMP = os.path.join(DEVKITPPC, "bin", "powerpc-eabi-objdump")
CPP = os.path.join(DEVKITPPC, "bin", "powerpc-eabi-cpp")

ICONV = f"{PYTHON} tools/sjis.py" # TODO: get actual iconv working(?)

#########
# Files #
#########

# Slices
DOL_SLICES = f"{CONFIG}/dol_slices.yml"

# Overrides (TODO: do these need to be separate for rel?)
ANALYSIS_OVERRIDES = f"{CONFIG}/analysis_overrides.yml"
DISASM_OVERRIDES = f"{MAIN_CONFIG}/disasm_overrides.yml"

# Binaries
DOL = f"{ORIG}/main.dol" # read in python code
DOL_YML = f"{CONFIG}/dol.yml"
DOL_SHA = f"{ORIG}/main.dol.sha1"
DOL_OK = f"{BUILDDIR}/main.dol.ok"
DOL_ASM_LIST = f"{BUILDDIR}/main.dol.asml"

# Symbols
SYMBOLS = f"{CONFIG}/symbols.yml"

# Assets
ASSETS_YML = f"{CONFIG}/assets.yml"

# Analysis outputs
EXTERNS = f"{BUILDDIR}/externs.pickle"
DOL_LABELS = f"{BUILDDIR}/labels.pickle"
DOL_RELOCS = f"{BUILDDIR}/relocs.pickle"

# Linker
DOL_LCF_TEMPLATE = f"{MAIN_CONFIG}/dol.lcf"
DOL_LCF = f"{BUILDDIR}/dol.lcf"

# Outputs
DOL_ELF = f"{BUILDDIR}/main.elf"
DOL_OUT = f"{OUTDIR}/main.dol"
DOL_MAP = f"{OUTDIR}/debugInfo{VERSION[0]}.MAP"

# Optional full disassembly
DOL_FULL = f"{OUTDIR}/dol.s"

DOL_SDATA2_SIZE = 4

# dol SHA1 Hash
DOL_SHA1_HASH = "f3bf225dd81cd9eb094fa9f8415f95f6bbcb9d10"  # PAL SHA1 Hash
if (VERSION == "MarioClub"):
    DOL_SHA1_HASH = "db87a9ec1a34275efc45d965dcdcb1a9eb131885"  # NTSC-U Debug SHA1 Hash

##############
# Tool Flags #
##############

ASFLAGS = ' '.join([
    "-m gekko",
    f"-I {INCDIR}",
    f"-I {PPCDIS_INCDIR}",
    f"-I orig"
])

INCDIRS = [
    PPCDIS_INCDIR,
    BUILD_INCDIR,
    "include",
    "libs/PowerPC_EABI_Support/include"
]
MWCC_INCLUDES = ' '.join(f"-i {d}" for d in INCDIRS)
GCC_INCLUDES = ' '.join(f"-I {d}" for d in INCDIRS)

#rework
DEFINES = [
    "DEBUG",
    "REGION_US",
    "MATCHING"
]
if REGION == "eu":
    DEFINES = [
        "VIDEO_PAL",
        "REGION_EU",
        "MATCHING"
    ]
MWCC_DEFINES = ' '.join(f"-d {d}" for d in DEFINES)
GCC_DEFINES = ' '.join(f"-D {d}" for d in DEFINES)

CPPFLAGS = ' '.join([
    "-nostdinc",
    GCC_DEFINES,
    GCC_INCLUDES
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
    "-use_lmw_stmw on",
    "-w off",
    "-common on",
    "-inline auto", 
    MWCC_DEFINES
]

JSYSTEM_SPEED = CFLAGS + [ "-O4,p" ]
JSYSTEM_RELEASE = CFLAGS + [ "-opt level=4, schedule"]
# confusion

MSL_C = [ 
    "-opt level=0, peephole, schedule, nospace",
    "-use_lmw_stmw on",
    "-fp hard",
    "-fp_contract on",
    "-Cpp_exceptions off",
    "-RTTI off",
    "-common off",
    "-str pool",
    "-rostr",
    "-inline off, deferred"
]

BASE_GAME_CFLAGS = CFLAGS + [ "-O4,s" ]
KANESHIGE = BASE_GAME_CFLAGS + [ "-inline off" ]

LOCAL_CFLAGS = [
    "-nostdinc",
    "-proc gekko",
    "-maxerrors 1",
    "-I-",
    MWCC_INCLUDES
]
DOL_CFLAGS = ' '.join(BASE_GAME_CFLAGS + LOCAL_CFLAGS)
MSL_C_CFLAGS = ' '.join(MSL_C + LOCAL_CFLAGS)

JSYSTEM_SPEED_CFLAGS = ' '.join(JSYSTEM_SPEED + LOCAL_CFLAGS)
JSYSTEM_RELEASE_CFLAGS = ' '.join(JSYSTEM_RELEASE + LOCAL_CFLAGS)
KANESHIGE_CFLAGS = ' '.join(BASE_GAME_CFLAGS + LOCAL_CFLAGS)
if (VERSION == "MarioClub"):
    KANESHIGE_CFLAGS = ' '.join(KANESHIGE + LOCAL_CFLAGS)
EXTERNAL_DOL_CFLAGS = ' '.join(BASE_GAME_CFLAGS)

LDFLAGS = ' '.join([
    "-fp hard",
    "-w off",
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

@dataclass
class SourceContext:
    cflags: str
    binary: str
    labels: str
    relocs: str
    slices: str
    sdata2_threshold: int

DOL_CTX = SourceContext(DOL_CFLAGS, DOL_YML, DOL_LABELS, DOL_RELOCS, DOL_SLICES, DOL_SDATA2_SIZE)

####################
# diff.py Expected #
####################

EXPECTED = f"expected/{VERSION_DIR}"
DOL_EXPECTED = f"{EXPECTED}/build/main.elf"