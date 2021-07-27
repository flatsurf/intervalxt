![Test](https://github.com/flatsurf/intervalxt/workflows/Test/badge.svg)
[![codecov](https://codecov.io/gh/flatsurf/intervalxt/branch/master/graph/badge.svg)](https://codecov.io/gh/flatsurf/intervalxt)
[![asv](http://img.shields.io/badge/benchmarked%20by-asv-blue.svg?style=flat)](https://flatsurf.github.io/intervalxt/asv/)
[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.4975225.svg)](https://doi.org/10.5281/zenodo.4975225)

## Interval Exchange Transformations

This repository provides an implementation of *Boshernitzan algorithm* using
interval exchange transformation. The aim is to compute the decomposition of a
given measured foliation into its periodic and minimal components. The delicate
part is of course to certify that a given component is minimal and this is
where the Boshernitzan's algorithm comes into play.

With the iet version we do not do the full decomposition but rather on one side
the union of minimal components and on the other side the periodic components.
Separating minimal components is not easily achieved with Rauzy induction.

If we do implement saddle connection search via flipping, we might want to
implement the decomposition by flipping. In that case we can separate the
minimal components and recover the full decomposition.

## Current Release Info

We release this package regularly with [rever](https://regro.github.io/rever-docs/index.html); typically with every push to the master branch.

This repository contains two related projects:

* **libintervalxt** a C++ library
* **pyintervalxt** a Python wrapper for **libintervalxt**

| Name | Downloads | Version | Platforms |
| --- | --- | --- | --- |
| [![Build](https://img.shields.io/badge/recipe-libintervalxt-green.svg)](https://anaconda.org/flatsurf/libintervalxt) | [![Conda Downloads](https://img.shields.io/conda/dn/flatsurf/libintervalxt.svg)](https://anaconda.org/flatsurf/libintervalxt) | [![Conda Version](https://img.shields.io/conda/vn/flatsurf/libintervalxt.svg)](https://anaconda.org/flatsurf/libintervalxt) | [![Conda Platforms](https://img.shields.io/conda/pn/flatsurf/libintervalxt.svg)](https://anaconda.org/flatsurf/libintervalxt) |
| [![Build](https://img.shields.io/badge/recipe-pyintervalxt-green.svg)](https://anaconda.org/flatsurf/pyintervalxt) | [![Conda Downloads](https://img.shields.io/conda/dn/flatsurf/pyintervalxt.svg)](https://anaconda.org/flatsurf/pyintervalxt) | [![Conda Version](https://img.shields.io/conda/vn/flatsurf/pyintervalxt.svg)](https://anaconda.org/flatsurf/pyintervalxt) | [![Conda Platforms](https://img.shields.io/conda/pn/flatsurf/pyintervalxt.svg)](https://anaconda.org/flatsurf/pyintervalxt) |

## Install with Conda

You can install this package with conda. Download and install [Mambaforge](https://github.com/conda-forge/miniforge#mambaforge), then run

```
mamba create -n intervalxt -c flatsurf libintervalxt pyintervalxt
conda activate intervalxt
```

## Run with binder in the Cloud

You can try out the projects in this repository in a very limited environment online by clicking the following links:

* **libintervalxt** [![Binder](https://mybinder.org/badge_logo.svg)](https://mybinder.org/v2/gh/flatsurf/intervalxt/master?filepath=doc%2Fbinder%2FSample.libintervalxt.ipynb)
* **pyintervalxt** [![Binder](https://mybinder.org/badge_logo.svg)](https://mybinder.org/v2/gh/flatsurf/intervalxt/master?filepath=doc%2Fbinder%2FSample.pyintervalxt.ipynb)

## Build from the Source Code Repository

We are following a standard autoconf setup, i.e., you can build intervalxt with
the following:

```
git clone --recurse-submodules https://github.com/flatsurf/intervalxt.git
cd intervalxt
./bootstrap
./configure
make
make check # to run our test suite
make install # to install into /usr/local
```

For best performance run `CXXFLAGS="-O3 -flto -march=native -mtune=native"
CXX="g++ -fno-semantic-interposition" ./configure` instead of `./configure` as
this code greatly benefits from flto inlining. (Unfortunately, libtool filters
out `-fno-semantic-interposition` as of early 2019 so we can not pass it as
part of `CXXFLAGS`. If you are using clang, `-fno-semantic-interposition` does
not seem to be necessary.) Do not use `-Ofast` or `-ffast-math` as parts of our
code rely on IEEE compliance. You might want to add `-g3` to the `CXXFLAGS`
which does not hurt performance but gives a better debugging experience. For
the best debugging experience, you might want to replace `-O3` with `-Og` or
even `-O0` but the latter results in very poor performance.

Additionally, you might want to run configure with ` --disable-static` which
improves the build time.

[perf](https://perf.wiki.kernel.org/index.php/Main_Page) works well to profile
when you make sure that `CXXFLAGS` contains `-fno-omit-framepointer`. You can
then for example run our test suite with `perf record --call-graph dwarf make
check` Apart from perf itself there are several ways to analyze the output,
[hotspot](https://github.com/KDAB/hotspot) might be the most convenient one at
the time of this writing.


## Build from the Source Code Repository with Conda Dependencies

To build this package, you need a fairly recent C++ compiler and probably some
packages that might not be readily available on your system. If you don't want
to use your distribution's packages, you can provide these dependencies with
conda. Download and install
[Mambaforge](https://github.com/conda-forge/miniforge#mambaforge), then run

```
git clone --recurse-submodules https://github.com/flatsurf/intervalxt.git
cd intervalxt

mamba create -n intervalxt-build
mamba env update -n intervalxt-build -f libintervalxt/environment.yml
mamba env update -n intervalxt-build -f pyintervalxt/environment.yml
conda activate intervalxt-build

./bootstrap
./configure
make
```

Note that the C++ compiler package from conda disabled all assertions. To
enable assertions `export CPPFLAGS="$CPPFLAGS -UNDEBUG"` before running
configure and make.

For faster compile times, you might want to `mamba install ccache` and set
`export CXX="ccache c++"` before running configure and make.

## Build from the Source Code Repository with Conda

The conda recipes in `{lib,py}intervalxt/recipe/` are built automatically as
part of our Continuous Integration. If you want to build the recipe manually,
something like the following should work:

```
git clone --recurse-submodules https://github.com/flatsurf/intervalxt.git
cd intervalxt
conda activate root
conda config --add channels flatsurf
mamba install conda-build
conda build libintervalxt/recipe pyintervalxt/recipe
```

You can then try out the package that you just built with:
```
mamba create -n intervalxt-test --use-local libintervalxt pyintervalxt
conda activate intervalxt-test
```

## Run Tests and Benchmark

`make check` runs all tests and benchmarks. During development `make check TESTS=module`
only runs the tests for `module`.

## How to Cite This Project

If you have used this project in the preparation of a publication, please cite
it as described [on our zenodo site](https://doi.org/10.5281/zenodo.4015050).

## Maintainers

* [@saraedum](https://github.com/saraedum)
* [@videlec](https://github.com/videlec)
