myhex.py
    usage: python myhex.py FILE

    outputs a hexdump of FILE in format: "one byte hex"
    with one byte per line

    warning: prints trailing spaces.

cmp.py
    compares two hexdumps of .bmp files outputted by myhex.py
    assumes dib header size is 12
    
    if one file is smaller, the excess of the other one is not outputted

run.sh
    runs the above