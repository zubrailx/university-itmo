.PHONY: clean

#-ggdb
CFLAGS= -std=c17 -pedantic -Wall -O3 -g -c

CC = gcc
LINKER = $(CC)
NASM = nasm

SOLUTION = solution

SRC = $(SOLUTION)/src
OBJECT = obj
TARGET = build

MKDIR = mkdir -p


all: $(OBJECT) $(TARGET)
	$(NASM) -felf64 -g $(SRC)/transform/sepia_asm.asm -o $(OBJECT)/sepia_asm.o

	$(CC) $(CFLAGS) $(SRC)/bmp/bmpio.c -o $(OBJECT)/bmpio.o
	$(CC) $(CFLAGS) $(SRC)/file/file.c -o $(OBJECT)/file.o
	$(CC) $(CFLAGS) $(SRC)/image/image.c -o $(OBJECT)/image.o
	$(CC) $(CFLAGS) $(SRC)/tool/tool.c -o $(OBJECT)/tool.o
	$(CC) $(CFLAGS) $(SRC)/transform/rotate.c -o $(OBJECT)/rotate.o
	$(CC) $(CFLAGS) $(SRC)/transform/sepia.c -o $(OBJECT)/sepia.o
	$(CC) $(CFLAGS) $(SRC)/tool/test.c -o $(OBJECT)/test.o
	$(CC) $(CFLAGS) $(SRC)/main.c -o $(OBJECT)/main.o

	$(LINKER) -g $(OBJECT)/*.o -o  $(TARGET)/main

$(OBJECT):
	$(MKDIR) $@

$(TARGET):
	$(MKDIR) $@

clean:
	rm -rf $(OBJECT) $(TARGET)

run:
	./$(TARGET)/main