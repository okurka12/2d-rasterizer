# porovnani bajtu bmp souboru
# porovnava dva soubory, ktere jsou vystupem `myhex.py`
# pokud je jeden soubor kratsi, 
# prebytek druheho se neporovnava
#
# POZOR: predpoklada ze dib header size je 12
#        a ze se jedna o OS/2 1.x BITMAPCOREHEADER
import sys

file_header = [
    "  // B",
    "  // M",
    "  // filesize LSB",
    "  // ..",
    "  // ..",
    "  // filesize MSB",
    "  // reserved",
    "  // reserved",
    "  // reserved",
    "  // reserved",
    "  // offset LSB",
    "  // ..",
    "  // ..",
    "  // offset MSB"
]

dib_header = [
    "  // dib_header_size LSB",
    "  // ..",
    "  // ..",
    "  // dib_header_size MSB",
    "  // width LSB",
    "  // width MSB",
    "  // height LSB",
    "  // height MSB",
    "  // number_of_color_planes LSB",
    "  // number_of_color_planes MSB",
    "  // bits_per_pixel LSB",
    "  // bits_per_pixel MSB"
]

# osetreni
if len(sys.argv) != 3:
    print("Wrong number of arguments")
    print(f"Usage: {sys.argv[0]} file1 file2")
    exit()

FILE_1 = sys.argv[1]
FILE_2 = sys.argv[2]

# nacteni prvniho hexdumpu
with open(FILE_1, "r") as f:
    a = f.readlines()

# nacteni druheho hexdumpu
with open(FILE_2, "r") as f:
    b = f.readlines()

# print hlavicky
print("funkcni | nefunkcni")

# iterace pres radky
for i in range(min(len(a), len(b))):

    # komentare bloku
    match i:
        case 0:
            print("/* file header */")
        case 14:
            print("\n/* dib header */")
        case 26:
            print("\n/* pixel array */")

    # bajty obou souboru
    funkcni = a[i].replace("\n", "")
    nefunkcni = b[i].replace("\n", "")
    print(f"{funkcni} | {nefunkcni}", end="")

    # komentare k bajtum hlavicek
    if 0 <= i <= 13:
        print(file_header[i])
    elif 14 <= i <= 25:
        print(dib_header[i - 14])
    else:
        print()  # '\n'
