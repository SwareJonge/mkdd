import argparse
import struct

def main():
    parser = argparse.ArgumentParser(
        description="This basically is bin2c"
    )
    parser.add_argument("address", help="Memory address (e.g., 0x80234500)")
    parser.add_argument("size", type=int, help="Number of elements")
    parser.add_argument("datatype", choices=[
        "s8", "u8", "s16", "u16", "s32", "u32", "s64", "u64", "f32", "f64"
    ], help="Data type of elements")
    args = parser.parse_args()

    fmt_map = {
        "s8": ("b", 1),  "u8": ("B", 1),
        "s16": ("h", 2), "u16": ("H", 2),
        "s32": ("i", 4), "u32": ("I", 4),
        "s64": ("q", 8), "u64": ("Q", 8),
        "f32": ("f", 4), "f64": ("d", 8),
    }
    fmt_char, type_size = fmt_map[args.datatype]
    total_bytes = args.size * type_size

    address = int(args.address, 16)
    dol_offset = address - 0x80003000 # lazy way to get get offset in the dol

    # open dol, jump to offset and read data
    with open("orig/MarioClub_us/sys/main.dol", "rb") as f:
        f.seek(dol_offset)
        data = f.read(total_bytes)

    fmt = f">{args.size}{fmt_char}"
    values = struct.unpack(fmt, data)

    # if datatype is only 8 bits, print 8 per line, otherwise print 4 
    vals_per_line = 4 if type_size != 1 else 8

    for i, val in enumerate(values):
        if i % vals_per_line == 0:
            print("    ", end="")
        if isinstance(val, float):
            print(f"{val:.8g}", end=", ")
        else:
            print(f"0x{val & ((1 << (type_size * 8)) - 1):0{type_size * 2}X}", end=", ")
        if i % vals_per_line == (vals_per_line - 1):
            print()
    if args.size % vals_per_line != 0:
        print()

if __name__ == "__main__":
    main()
