.PHONY: all clean run build main

CURRENT_DIR = $(shell pwd)

DATA_DIR    = data
BUILD_DIR   = build
OUTPUT_DIR  = output
INCLUDE_DIR = include
SRC_DIR     = src

CC = gcc
CFLAGS = -Wall -g
WCC=i686-w64-mingw32-gcc


all: data output main.build

clean: main.clean data.clean output.clean

run: all main.run


# MAIN
main.run: main.build
	./$(BUILD_DIR)/main	


SRC_FILES   =   $(wildcard $(SRC_DIR)/*.c)
HEADER_FILES=   $(wildcard $(INCLUDE_DIR)/*.h)
OBJECT_FILES= $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

main.build: main.dir $(BUILD_DIR)/main
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/main: $(OBJECT_FILES) $(SRC_DIR)/main.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@


main.dir:
	mkdir -p $(BUILD_DIR)

main.clean:
	rm -rf $(BUILD_DIR)


# DATA
data: $(DATA_DIR)/input.exe

data.run: $(DATA_DIR)/input.exe
	wine $<

$(DATA_DIR)/input.exe: $(DATA_DIR)/input.c
	$(WCC) $< -o $@

data.clean: 
	rm $(DATA_DIR)/input.exe


# OUTPUT
output: output.dir

output.dir:
	mkdir -p $(OUTPUT_DIR)/sections

output.clean:
	rm -rf $(OUTPUT_DIR)