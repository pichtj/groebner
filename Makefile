BUILDDIR := $(shell pwd)
CFLAGS := -m64 -g -pg -O3 -march=native -mtune=native -I$(BUILDDIR)/include -L$(BUILDDIR)/lib -std=c++11 -lmpir -lmpirxx -lgmp -Wall
CPPFLAGS := -DDEBUG
CC := $(shell which gcc-mp-4.9 || echo gcc)
CXX := $(shell which g++-mp-4.9 || echo g++)
MPIR := mpir-2.7.0
GTEST := gtest-1.7.0

all: moGVW test

moGVW: main.o Monomial.o lib/libmpirxx.a lib/libmpir.a
	$(CXX) $(CFLAGS) $(CPPFLAGS) -o $@ $^ -lmpirxx -lmpir

main.o: main.cpp Polynomial.h LabelledMonomial.h include/mpir.h include/mpirxx.h
	$(CXX) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

clean:
	rm -rf moGVW *.o test-runner

distclean: clean
	rm -rf include lib share $(MPIR)* $(GTEST)*

$(MPIR).tar.bz2:
	wget http://mpir.org/$(MPIR).tar.bz2

$(MPIR): $(MPIR).tar.bz2
	tar jxvf $<

lib/libmpir.a include/mpir.h include/mpirxx.h: $(MPIR)
	cd $< && CC="$(CC)" ./configure --enable-gmpcompat --enable-cxx --disable-shared --enable-static --prefix=$(BUILDDIR) && make && make install

$(GTEST).zip:
	wget https://googletest.googlecode.com/files/$(GTEST).zip

$(GTEST): $(GTEST).zip
	test -e $@ || unzip $<

libs: lib/libmpir.a

test: test-runner
	./test-runner

%Test.o: %Test.cpp %.h $(GTEST)
	$(CXX) $(CFLAGS) $(CPPFLAGS) -c -o $@ $< -isystem $(GTEST)/include

gtest-all.o: $(GTEST)/src/gtest-all.cc
	$(CXX) $(CFLAGS) $(CPPFLAGS) -isystem $(GTEST)/include -I$(GTEST) -pthread -c $<

gtest_main.o: $(GTEST)/src/gtest_main.cc
	$(CXX) $(CFLAGS) $(CPPFLAGS) -isystem $(GTEST)/include -I$(GTEST) -pthread -c $<

TEST_OBJECTS := $(shell ls *Test.cpp | sed -e s/cpp$$/o/g)

test-runner: $(TEST_OBJECTS) gtest-all.o gtest_main.o Monomial.o
	$(CXX) $^ $(CFLAGS) -pthread -o test-runner

%.o: %.cpp
	$(CXX) $(CFLAGS) $(CPPFLAGS) -c -o $@ $<

