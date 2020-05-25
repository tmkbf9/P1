.PHONY: clean

GCC := g++
CFLAGS := -g
RM := rm

.cpp.o:
	$(GCC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<

all: tests

clean:
	$(RM) -f *.o
	$(RM) -f *~
	$(RM) -f scanner_test


tests: scanner_test.o scanner.o
