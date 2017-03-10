# groebner

This repository contains algorithms for groebner basis computation:
* `moGVW`, my own implementation of Sun, Wang, Huang and Lins monomial-oriented groebner GVW for computing groebner bases
* `F5`, my own implementation of Faugères F5 algorithm
* `FGb`, a wrapper around Faugères [fast groebner library](http://www-polsys.lip6.fr/~jcf/FGb/index.html)

## Papers

* [2014 Sun, Wang, Huang, Lin - A Monomial-oriented GVW for Computing Gröbner Bases](https://arxiv.org/abs/1410.0105v1)
* [2010 Faugère - FGb/ a library for computing Gröbner bases](http://www-polsys.lip6.fr/~jcf/Papers/ICMS.pdf)
* 2010 Eder, Perry - F5C A variant of Faugère's F5 algorithm with reduced Gröbner bases
* [2010 Albrecht, Perry - F45](https://arxiv.org/abs/1006.4933v2)
* [2002 Faugère - A new efficient algorithm for computing Gröbner bases without reduction to zero (F5)](http://www-polsys.lip6.fr/~jcf/Papers/F02a.pdf)
* [1999 Faugère - A new efficient algorithm for computing Gröbner bases (F4)](http://www-polsys.lip6.fr/~jcf/Papers/F99a.pdf)

# Installation 

## Mac OS X

Currently, groebner only compiles with gcc, so install that first. E.g. with `homebrew`:

```
brew install gcc --without-multilib
```

`--without-multilib` is needed to avoid an issue with static linking and `-fopenmp`.

Then, start the build process:

```
make
```

This should download and compile some libraries and eventually compile the programs.

## Vagrant/Ubuntu 

Running `vagrant up` in a checkout should create an Ubuntu 14.04 image with all necessary software installed. You can then enter the VM and start the build process:

```
vagrant ssh
cd moGVW
make
```
