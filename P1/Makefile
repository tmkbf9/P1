.PHONY: clean compile test

GCC := g++
CFLAGS := -g
CPPFLAGS := --std=c++98
RM := rm

.cpp.o:
	$(GCC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<

all: P1 test

P1: main.o testScanner.o scanner.o token.o
	$(GCC) -o $@ $^

test: scanner_test
	./scanner_test

scanner_test: scanner_test.o scanner.o token.o
	$(GCC) -o $@ $^

clean:
	$(RM) -f P1
	$(RM) -f scanner_test
	$(RM) -f *.o
	$(RM) -f *~
