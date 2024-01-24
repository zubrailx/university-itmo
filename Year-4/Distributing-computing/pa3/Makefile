.PHONY = build run clean

ARCH := 64
LIBRARIES_PATH := $(PWD)/lib$(ARCH)

# If the first argument is "run"...
ifeq (run,$(firstword $(MAKECMDGOALS)))
  # use the rest as arguments for "run"
  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  # ...and turn them into do-nothing targets
  $(eval $(RUN_ARGS):;@:)
endif

SRCS := $(wildcard *.c)
PROG := a.out
LOGS := $(wildcard *.log)

$(PROG): $(SRCS)
	clang -g -std=c99 -Wall -pedantic $^ -L$(LIBRARIES_PATH) -lruntime -o $@

build: $(PROG)

run: $(PROG)
	LD_LIBRARY_PATH=$(LIBRARIES_PATH) $(PWD)/$< $(RUN_ARGS)

clean:
	rm -f a.out $(LOGS)
