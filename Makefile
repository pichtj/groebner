BUILDDIR := $(shell pwd)
FGB_LIBDIR := $(BUILDDIR)/call_FGb/nv/maple/C/$(shell uname | grep Linux >/dev/null && echo x64 || echo macosx)
CXXFLAGS := -std=c++11 -m64 -O3 -Wall
LDFLAGS := -L$(BUILDDIR)/lib -lflint -lmpir -lmpfr -lmpirxx -lgmp -fopenmp -pthread -lpng
FGB_LDFLAGS := -L$(FGB_LIBDIR) -lfgb -lfgbexp -lgb -lgbexp -lminpoly -lminpolyvgf -lgmp -lm
CPPFLAGS := -I$(BUILDDIR)/include -I$(BUILDDIR)/include/flint -DPNG_OUTPUT
FGB_CPPFLAGS := -I$(BUILDDIR)/call_FGb/nv/protocol -I$(BUILDDIR)/call_FGb/nv/int -I$(BUILDDIR)/call_FGb/nv/maple/C -Wno-write-strings -Wno-unused-but-set-variable -Wno-unused-function
CC := $(shell which gcc-mp-4.9 || echo gcc)
CXX := $(shell which g++-mp-4.9 || echo g++)
MPIR := mpir-2.7.0
GTEST := gtest-1.7.0
FLINT := flint-2.5.2
MPFR := mpfr-3.1.3

all: moGVW F5 FGb test

moGVW: moGVW.o Monomial.o Ideal.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

moGVW.o: moGVW.cpp *.h include/mpir.h include/mpirxx.h include/flint/fmpz.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<

clean:
	rm -rf moGVW *.o *.dSYM test-runner FGb F5 gmon.out

distclean: clean
	rm -rf include lib share $(MPIR) $(MPFR) $(FLINT) $(GTEST) call_FGb

.downloads/$(MPIR).tar.bz2:
	mkdir -p .downloads && cd .downloads && wget --continue http://mpir.org/$(MPIR).tar.bz2

$(MPIR): .downloads/$(MPIR).tar.bz2
	tar jxvf $<

lib/libmpir.a include/mpir.h include/mpirxx.h lib/libgmp.a include/gmp.h: $(MPIR)
	cd $< && CC="$(CC)" ./configure --enable-gmpcompat --enable-cxx --disable-shared --enable-static --prefix=$(BUILDDIR) && make && make install

.downloads/$(MPFR).tar.bz2:
	mkdir -p .downloads && cd .downloads && wget --continue http://www.mpfr.org/mpfr-current/$(MPFR).tar.bz2

$(MPFR): .downloads/$(MPFR).tar.bz2
	tar jxvf $<

lib/libmpfr.a include/mpfr.h: $(MPFR) lib/libgmp.a
	cd $< && CC="$(CC)" ./configure --disable-shared --with-gmp=$(BUILDDIR) --prefix=$(BUILDDIR) && make && make install

.downloads/$(FLINT).tar.gz:
	mkdir -p .downloads && cd .downloads && wget --continue http://www.flintlib.org/$(FLINT).tar.gz

$(FLINT): .downloads/$(FLINT).tar.gz
	tar zxvf $<

lib/libflint.a include/flint/flint.h include/flint/fmpz.h: $(FLINT) lib/libmpfr.a
	cd $< && CC="$(CC)" ./configure --with-mpir=$(BUILDDIR) --with-gmp=$(BUILDDIR) --disable-shared --enable-cxx --prefix=$(BUILDDIR) && make && make install

.downloads/call_FGb6.maclinux.x64.tar.gz:
	mkdir -p .downloads && cd .downloads && wget --continue http://www-polsys.lip6.fr/~jcf/FGb/C/@downloads/call_FGb6.maclinux.x64.tar.gz

call_FGb: .downloads/call_FGb6.maclinux.x64.tar.gz
	test -e $@ || tar zxvf $<

.downloads/$(GTEST).zip:
	mkdir -p .downloads && cd .downloads && wget --continue https://googletest.googlecode.com/files/$(GTEST).zip

$(GTEST): .downloads/$(GTEST).zip
	test -e $@ || unzip $<

libs: lib/libmpir.a lib/libflint.a

test: test-runner
	./test-runner

%Test.o: %Test.cpp %.h $(GTEST)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $< -isystem $(GTEST)/include

gtest-all.o: $(GTEST)/src/gtest-all.cc
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -isystem $(GTEST)/include -I$(GTEST) -c $<

gtest_main.o: $(GTEST)/src/gtest_main.cc
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -isystem $(GTEST)/include -I$(GTEST) -c $<

TEST_OBJECTS := $(shell ls *Test.cpp | sed -e s/cpp$$/o/g)

test-runner: $(TEST_OBJECTS) gtest-all.o gtest_main.o Monomial.o Ideal.o
	$(CXX) $^ -o test-runner $(LDFLAGS) $(FGB_LDFLAGS)

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<

FGb.o: FGb.cpp FGb.h Monomial.h Polynomial.h call_FGb
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(FGB_CPPFLAGS) -c -o $@ $<

FGbTest.o: FGbTest.cpp FGb.h Monomial.h Polynomial.h call_FGb
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(FGB_CPPFLAGS) -c -o $@ $< -isystem $(GTEST)/include

FGb: FGb.o Monomial.o Ideal.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(FGB_LDFLAGS)

F5: F5.o Monomial.o Ideal.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.mp4:
	ffmpeg -framerate 25 -pattern_type glob -i '$**.png' -vf scale='flags=neighbor:w=trunc((iw+1)/2)*2:h=trunc((ih+1)/2)*2' -c:v libx264 -pix_fmt yuv420p $@
