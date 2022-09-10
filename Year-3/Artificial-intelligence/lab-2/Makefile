NAME := exec

CXX = g++
LD = $(CXX)

CXXFLAGS.debug = $(strip $(file < compile_flags.txt)) -g
CXXFLAGS = $(CXXFLAGS.debug)
LDFLAGS = $(CXXFLAGS)

RM = rm -rf
MKDIR = mkdir -p

SRCDIR = src
BUILDDIR = build
OBJDIR = $(BUILDDIR)
TARGET = $(BUILDDIR)/$(NAME)

SOURCES := $(shell find $(SRCDIR) -type f -name '*.cpp')
OBJECTS := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
DIRS := $(sort $(dir $(OBJECTS)))

.PHONY: all test clean build

all: build

run:
	exec $(TARGET)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

build: $(DIRS) $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $(TARGET)

clean:
	$(RM) $(BUILDDIR)

$(sort $(DIRS)):
	@$(MKDIR) $@
