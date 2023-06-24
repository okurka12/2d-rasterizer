# moje jednoducha verze hexdumpu protoze jsem neprisel na to jak
# s hexdumpem udelat to co chci (one byte hex, one byte per line)
#
# POZOR: zanechava koncove mezery!

import sys

# load file
with open(sys.argv[1], "br") as f:
    contents = f.read()

# return ascii char if it's ascii and printable, else return a space
def char_str(a: int) -> str:
    b = chr(a)
    if b.isprintable() and a < 128:
        return b
    else:
        return " "

# for each byte
for i, byte in enumerate(contents):
    print(
        str(i).rjust(8, "0"),  # number
        hex(byte).replace('0x' , '').rjust(2, "0"),  # byte hex
        char_str(byte)  # byte ascii if it's ascii and printable
    )
