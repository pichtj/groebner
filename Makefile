BUILDDIR := $(shell pwd)
CFLAGS := -m64 -I$(BUILDDIR)/include -L$(BUILDDIR)/lib -std=c++14
CPPFLAGS :=
CC := $(shell which gcc-mp-4.9 || echo gcc)
CXX := $(shell which g++-mp-4.9 || echo g++)

all: moGVW

moGVW: main.o lib/libmpirxx.a lib/libmpir.a
	$(CXX) $(CFLAGS) -o $@ $^ -lmpirxx -lmpir

main.o: main.cpp Polynomial.h LabelledMonomial.h include/mpir.h include/mpirxx.h
	$(CXX) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

clean:
	rm -rf moGVW *.o

distclean: clean
	rm -rf include lib share mpir-2.7.0*

mpir-2.7.0.tar.bz2:
	wget http://mpir.org/mpir-2.7.0.tar.bz2

mpir-2.7.0: mpir-2.7.0.tar.bz2
	tar jxvf $<

lib/libmpir.a include/mpir.h include/mpirxx.h: mpir-2.7.0
	cd $< && CC="$(CC)" ./configure --enable-cxx --disable-shared --enable-static --prefix=$(BUILDDIR) && make && make install

libs: lib/libmpir.a
