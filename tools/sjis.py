from argparse import ArgumentParser
import shutil

parser = ArgumentParser()
parser.add_argument("input")
parser.add_argument("output")
args = parser.parse_args()

try:
    with open(args.input, encoding="utf-8") as f:
        txt = f.read()
    with open(args.output, 'w', encoding="shift-jis") as f:
        f.write(txt)
except: # if source file already isn't utf-8 assume it's shift jis and copy
    shutil.copyfile(args.input, args.output)
