CFLAGS := -m64 -L/opt/local/lib
CPPFLAGS := -std=c++14
CC := $(shell which gcc-mp-4.9 || echo gcc) $(CFLAGS)
CPP := $(shell which g++-mp-4.9 || echo g++) $(CFLAGS) $(CPPFLAGS)

all: moGVW

moGVW: main.o
	$(CPP) -o $@ $^ -lgmp

main.o: main.cpp Polynomial.h LabelledMonomial.h
	$(CPP) -c -o $@ $<

clean:
	rm -rf moGVW *.o
