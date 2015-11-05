BUILDDIR := $(shell pwd)
CFLAGS := -std=c++11 -m64 -pg -O3 -march=native -mtune=native -I$(BUILDDIR)/include -I$(BUILDDIR)/include/flint -Wall
LDFLAGS := -L$(BUILDDIR)/lib -lflint -lmpir -lmpfr -lmpirxx -lgmp -lpng -pthread
CPPFLAGS := -DINFO
CC := gcc
CXX := g++
MPIR := mpir-2.7.0
GTEST := gtest-1.7.0
FLINT := flint-2.5.2
MPFR := mpfr-3.1.3

all: moGVW test

moGVW: main.o Monomial.o lib/libmpirxx.a lib/libmpir.a
	$(CXX) $(CFLAGS) $(CPPFLAGS) -o $@ $^ $(LDFLAGS)

main.o: main.cpp Polynomial.h include/mpir.h include/mpirxx.h
	$(CXX) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

clean:
	rm -rf moGVW *.o test-runner

distclean: clean
	rm -rf include lib share $(MPIR)* $(MPFR)* $(FLINT)* $(GTEST)*

$(MPIR).tar.bz2:
	wget http://mpir.org/$(MPIR).tar.bz2

$(MPIR): $(MPIR).tar.bz2
	tar jxvf $<

lib/libmpir.a include/mpir.h include/mpirxx.h lib/libgmp.a include/gmp.h: $(MPIR)
	cd $< && CC="$(CC)" ./configure --enable-gmpcompat --enable-cxx --disable-shared --enable-static --prefix=$(BUILDDIR) && make && make install

$(MPFR).tar.bz2:
	wget http://www.mpfr.org/mpfr-current/$(MPFR).tar.bz2

$(MPFR): $(MPFR).tar.bz2
	tar jxvf $<

lib/libmpfr.a include/mpfr.h: $(MPFR) lib/libgmp.a
	cd $< && ./configure --disable-shared --with-gmp=$(BUILDDIR) --prefix=$(BUILDDIR) && make && make install

$(FLINT).tar.gz:
	wget http://www.flintlib.org/$(FLINT).tar.gz

$(FLINT): $(FLINT).tar.gz
	tar zxvf $<

lib/libflint.a include/flint/flint.h: $(FLINT) lib/libmpfr.a
	cd $< && ./configure --with-mpir=$(BUILDDIR) --with-gmp=$(BUILDDIR) --disable-shared --enable-cxx --prefix=$(BUILDDIR) && make && make install

$(GTEST).zip:
	wget https://googletest.googlecode.com/files/$(GTEST).zip

$(GTEST): $(GTEST).zip
	test -e $@ || unzip $<

libs: lib/libmpir.a lib/libflint.a

test: test-runner
	./test-runner

%Test.o: %Test.cpp %.h $(GTEST)
	$(CXX) $(CFLAGS) $(CPPFLAGS) -c -o $@ $< -isystem $(GTEST)/include

gtest-all.o: $(GTEST)/src/gtest-all.cc
	$(CXX) $(CFLAGS) $(CPPFLAGS) -isystem $(GTEST)/include -I$(GTEST) -c $<

gtest_main.o: $(GTEST)/src/gtest_main.cc
	$(CXX) $(CFLAGS) $(CPPFLAGS) -isystem $(GTEST)/include -I$(GTEST) -c $<

TEST_OBJECTS := $(shell ls *Test.cpp | sed -e s/cpp$$/o/g)

test-runner: $(TEST_OBJECTS) gtest-all.o gtest_main.o Monomial.o
	$(CXX) $^ $(CFLAGS) -o test-runner $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

