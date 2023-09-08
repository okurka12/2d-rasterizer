
CC=gcc
AR=ar
PY=python3
CFLAGS=-std=c11 -Wall -Wextra -pedantic -g #-fsanitize=address -DNDEBUG
LDFLAGS=

BMP_LIB_MODULES= bmp_save.o bmp_image_ctor.o bmp_image_dtor.o bmp_draw_rect.o \
 bmp_draw_circ.o bmp_draw_line.o

.PHONY: all
all: example.bin

# compile example.bin 
example.bin: libbmap.so example.c utils.h bmp.h
	$(CC) $(CFLAGS) -o $@ example.c ./libbmap.so 

# put bmp module into a static library
libbmap.a: $(BMP_LIB_MODULES)
	$(AR) r $@ $^

# put bmp module into a dynamic library
libbmap.so: $(BMP_LIB_MODULES)
	$(CC) $(LDFLAGS) -shared -fPIC -o $@ $^

# compile bmp_save
bmp_save.o: bmp_save.c
	$(CC) $(CFLAGS) -fPIC -c $<

# compile bmp_image_ctor
bmp_image_ctor.o: bmp_image_ctor.c
	$(CC) $(CFLAGS) -fPIC -c $<

# compile bmp_image_dtor
bmp_image_dtor.o: bmp_image_dtor.c
	$(CC) $(CFLAGS) -fPIC -c $<

# compile bmp_draw_square
bmp_draw_rect.o: bmp_draw_rect.c
	$(CC) $(CFLAGS) -fPIC -c $<

# compile bmp_draw_circ
bmp_draw_circ.o: bmp_draw_circ.c
	$(CC) $(CFLAGS) -fPIC -c $<

 # compile bmp_draw_line
bmp_draw_line.o: bmp_draw_line.c
	$(CC) $(CFLAGS) -fPIC -c $<

.PHONY: clean
clean:
	rm -f *.o *.bin *.so *.a *.bmp *.obj *.exe *.dll *.exp *.lib *.pdb *.ilk

.PHONY: run
run: run_wsl

# Runs the demo (example.c) and opens the image
# Why does this even work? 
# WSL's PATH contains also the contents of Windows' PATH
# This means when you try to execute a Windows executable from WSL terminal,
# it is actually run in Windows. Windowed applications start normally, CLI 
# applications' input/output is tied to the WSL shell
.PHONY: run_wsl
run_wsl: example.bin
	./example.bin
	cmd.exe /c start example.bmp

# runs the python wrapper demo
.PHONY: run_python
run_python: libbmap.so
	$(PY) ./python/libbmap.py

# opens the image from the python wrapper demo
.PHONY: run_python_wsl
run_python_wsl: run_python
	cmd.exe /c start python_example.bmp
