BUILDDIR := $(shell pwd)
CC := $(shell which gcc-7 || which gcc-4.8 || echo gcc)
CXX := $(shell which g++-7 || which g++-4.8 || echo g++)
FGB_LIBDIR := $(BUILDDIR)/call_FGb/nv/maple/C/$(shell uname | grep Linux >/dev/null && echo x64 || echo macosx)
CXXFLAGS := -std=c++11 -m64 -O3 -Wall
LDFLAGS := -L$(BUILDDIR)/lib -lflint -lmpir -lmpfr -lmpirxx -lgmp $(shell $(CC) -v 2>&1 | grep gcc >/dev/null && echo -fopenmp) -pthread -lpng -lz
FGB_LDFLAGS := -L$(FGB_LIBDIR) $(shell uname | grep Linux >/dev/null && echo -Wl,-allow-multiple-definition) -lfgb -lfgbexp -lgb -lgbexp -lminpoly -lminpolyvgf -lgmp -lm
CPPFLAGS := -I$(BUILDDIR)/include -I$(BUILDDIR)/include/flint -DINFO
FGB_CPPFLAGS := -I$(BUILDDIR)/call_FGb/nv/protocol -I$(BUILDDIR)/call_FGb/nv/int -I$(BUILDDIR)/call_FGb/nv/maple/C -Wno-write-strings -Wno-unused-but-set-variable -Wno-unused-function
MPIR := mpir-3.0.0
GTEST_VERSION := release-1.7.0
GTEST := googletest-$(GTEST_VERSION)
FLINT := flint-2.5.2
MPFR := mpfr-4.0.0
PNG := libpng-1.6.34

all: moGVW F5 FGb interreduce intercept.so test

moGVW: moGVW.o Monomial.o Ideal.o Polynomial.o debug.o integral.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

moGVW.o: moGVW.cpp *.h include/mpir.h include/mpirxx.h include/flint/fmpz.h include/png.h lib/libpng.a
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<

clean:
	rm -rf moGVW *.o *.dSYM test-runner FGb F5 gmon.out

distclean: clean
	rm -rf include lib share bin $(MPIR) $(MPFR) $(FLINT) googletest-$(GTEST_VERSION) call_FGb

.downloads/$(MPIR).tar.bz2:
	mkdir -p .downloads && cd .downloads && wget --continue http://mpir.org/$(MPIR).tar.bz2

$(MPIR): .downloads/$(MPIR).tar.bz2
	tar jxf $<

lib/libmpir.a include/mpir.h include/mpirxx.h lib/libgmp.a include/gmp.h: $(MPIR)
	cd $< && CC="$(CC)" ./configure --enable-gmpcompat --enable-cxx --disable-shared --enable-static --prefix=$(BUILDDIR) && make && make install

.downloads/$(MPFR).tar.bz2:
	mkdir -p .downloads && cd .downloads && wget --continue http://www.mpfr.org/mpfr-current/$(MPFR).tar.bz2

$(MPFR): .downloads/$(MPFR).tar.bz2
	tar jxf $<

lib/libmpfr.a include/mpfr.h: $(MPFR) lib/libgmp.a
	cd $< && CC="$(CC)" ./configure --disable-shared --with-gmp=$(BUILDDIR) --prefix=$(BUILDDIR) && make && make check && make install

.downloads/$(FLINT).tar.gz:
	mkdir -p .downloads && cd .downloads && wget --continue http://www.flintlib.org/$(FLINT).tar.gz

$(FLINT): .downloads/$(FLINT).tar.gz
	tar zxf $<

lib/libflint.a include/flint/flint.h include/flint/fmpz.h include/flint/fmpzxx.h: $(FLINT) lib/libmpfr.a
	cd $< && CC="$(CC)" ./configure --with-mpir=$(BUILDDIR) --with-gmp=$(BUILDDIR) --disable-shared --enable-cxx --prefix=$(BUILDDIR) && make && make install

.downloads/call_FGb6.maclinux.x64.tar.gz:
	mkdir -p .downloads && cd .downloads && wget --continue http://www-polsys.lip6.fr/~jcf/FGb/C/@downloads/call_FGb6.maclinux.x64.tar.gz

call_FGb: .downloads/call_FGb6.maclinux.x64.tar.gz
	test -e $@ || tar zxf $<

.downloads/$(GTEST_VERSION).zip:
	mkdir -p .downloads && cd .downloads && wget --continue https://github.com/google/googletest/archive/$(GTEST_VERSION).zip

$(GTEST) $(GTEST)/include/gtest/gtest.h: .downloads/$(GTEST_VERSION).zip
	test -e $@ || unzip $<

.downloads/$(PNG).tar.gz:
	mkdir -p .downloads && cd .downloads && wget --continue http://prdownloads.sourceforge.net/libpng/$(PNG).tar.gz

$(PNG): .downloads/$(PNG).tar.gz
	test -e $@ || tar zxf $<

lib/libpng.a include/png.h: $(PNG)
	cd $(PNG) && CC=$(CC) ./configure --prefix=$(BUILDDIR) --disable-shared && make && make install

libs: lib/libmpir.a lib/libflint.a lib/libpng.a

test: test-runner
	./test-runner

%Test.o: %Test.cpp %.h include/mpirxx.h include/flint/fmpzxx.h $(GTEST)/include/gtest/gtest.h lib/libpng.a
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -isystem $(GTEST)/include -o $@ $<

gtest-all.o: $(GTEST)/src/gtest-all.cc
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -isystem $(GTEST)/include -I$(GTEST) -c $<

gtest_main.o: $(GTEST)/src/gtest_main.cc
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -isystem $(GTEST)/include -I$(GTEST) -c $<

TEST_OBJECTS := $(shell ls *Test.cpp | sed -e s/cpp$$/o/g)

test-runner: $(TEST_OBJECTS) gtest-all.o gtest_main.o Monomial.o Ideal.o Polynomial.o debug.o integral.o
	$(CXX) $^ -o test-runner $(LDFLAGS) $(FGB_LDFLAGS)

%.o: %.cpp %.h lib/libpng.a
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<

FGb.o: FGb.cpp FGb.h Monomial.h Polynomial.h call_FGb lib/libpng.a
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(FGB_CPPFLAGS) -c -o $@ $<

FGbTest.o: FGbTest.cpp FGb.h Monomial.h Polynomial.h call_FGb lib/libpng.a
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(FGB_CPPFLAGS) -c -o $@ $< -isystem $(GTEST)/include

FGb: FGb.o Monomial.o Ideal.o Polynomial.o debug.o integral.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) $(FGB_LDFLAGS)

F5: F5.o Monomial.o Ideal.o Polynomial.o debug.o integral.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

interreduce: interreduce.o Monomial.o Ideal.o Polynomial.o debug.o integral.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.mp4:
	ffmpeg -framerate 25 -pattern_type glob -i '$**.png' -vf scale='flags=neighbor:w=trunc((iw+1)/2)*2:h=trunc((ih+1)/2)*2' -c:v libx264 -pix_fmt yuv420p $@

%.so: %.c
	$(CC) $(CFLAGS) -fPIC -shared $(shell uname | grep Linux >/dev/null && echo -Wl,--no-as-needed) -ldl -o $@ $<
