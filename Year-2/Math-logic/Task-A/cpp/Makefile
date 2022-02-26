CXXFLAGS+=-std=c++14 -Wall -O2
CXX=g++

all: out out/main.o

run: out out/main.o
	out/main.o

out/main.o: src/main.cpp src/parser/expression.lexer.c src/parser/expression.tab.c
	$(CXX) $^ $(CXXFLAGS) -o out/main.o

pack: src/parser/expression.lexer.c src/parser/expression.tab.c
	zip task1.zip -r Makefile src/

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

