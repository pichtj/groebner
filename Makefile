CFLAGS := -m64 -L/opt/local/lib
CC := $(shell which gcc-mp-4.9 || echo gcc) $(CFLAGS)
CPP := $(shell which g++-mp-4.9 || echo g++) $(CFLAGS)

all: moGVW

moGVW: main.o
	$(CPP) -o $@ $^ -lgmp

main.o: main.cpp polynomial.h lmonomial.h
	$(CPP) -c -o $@ $<

clean:
	rm -rf moGVW *.o
