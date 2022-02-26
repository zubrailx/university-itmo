CXXFLAGS+=-std=c++14 -Wall -ggdb -g
CXX=g++

SOURCES=src/main.cpp src/util/util.cpp src/util/trim.cpp src/compute/axiom.cpp src/compute/hipothesis.cpp \
src/compute/proof.cpp


.PHONY: clean out pack

all: out out/main

run: out out/main
	out/main

out/main: $(SOURCES) src/parser/expression.lexer.c src/parser/expression.tab.c
	$(CXX) $(CXXFLAGS) -O2 -o out/main $^


pack: src/parser/expression.lexer.c src/parser/expression.tab.c
	zip taskC.zip -r Makefile src/

src/parser/%.lexer.c src/parser/%.lexer.h: src/parser/%.lex
	flex src/parser/$*.lex
	mv lex.c src/parser/$*.lexer.c
	mv lex.h src/parser/$*.lexer.h

src/parser/%.tab.c src/parser/%.tab.h: src/parser/%.y
	bison -d -v $^ -o src/parser/$*.tab.c


out:
	mkdir -p out/

clean:
	rm -rf out/
	rm -f src/parser/*.c src/parser/*.h
