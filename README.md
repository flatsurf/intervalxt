<p align="center">
    <img alt="logo" src="https://github.com/flatsurf/intervalxt/raw/master/logo.svg?sanitize=true" width="500px">
</p>

<h1><p align="center">intervalxt</p></h1>

<p align="center">
  <img src="https://img.shields.io/badge/License-GPL_3.0_or_later-blue.svg" alt="License: GPL 3.0 or later">
  <a href="https://github.com/flatsurf/intervalxt/actions/workflows/test.yml"><img src="https://github.com/flatsurf/intervalxt/actions/workflows/test.yml/badge.svg" alt="Test"></a>
  <a href="https://codecov.io/gh/flatsurf/intervalxt"><img src="https://codecov.io/gh/flatsurf/intervalxt/branch/master/graph/badge.svg" alt="Coverage"></a>
  <a href="https://flatsurf.github.io/intervalxt/asv"><img src="https://img.shields.io/badge/benchmarked%20by-asv-blue.svg?style=flat" alt="ASV"></a>
  <a href="https://doi.org/10.5281/zenodo.4015050"><img src="https://zenodo.org/badge/DOI/10.5281/zenodo.5166953.svg" alt="DOI 10.5281/zenodo.4015050"></a>
</p>

<p align="center">Interval Exchange Transformations</p>
<hr>

intervalxt is a C++/Python library which implements algorithms on [Interval Exchange Transformations (IETs)](https://en.wikipedia.org/wiki/Interval_exchange_transformation).

In particular, we implement the *Boshernitzan Algorithm* and *Rauzy Induction*
on IETs. Our principal application is the decomposition of a measured foliation
into periodic and minimal components. We certify that a component is minimal
with the Boshernitzan Algorithm or by certifying that the Rauzy Induction will
be looping infinitely. Note that presently the algorithm is probably
incomplete; in rare cases no further decomposition of a component seems to be
possible, nevertheless, our criteria fail to certify minimality of the
component.

Source tarballs can be downloaded at https://github.com/flatsurf/intervalxt/releases.

This repository contains two related projects:

* **libintervalxt** a C++ library
* **pyintervalxt** a Python wrapper for **libintervalxt**

## Build from the Source Code Repository or a Tarball

If you have cloned the source directory you will need to setup the
configure script and Makefile using autotools. That is

    git submodule update --init
    ./bootstrap

If you obtained a tarball of the sources or if the preceding step
worked, you just have to do

    ./configure
    make
    make check  # to run our test suite
    make install  # to install into /usr/local

If you happen to have any of the dependencies installed in a non-standard
directory, you will have to specify the `CPPFLAGS` and `LDFLAGS` variables for
the configure script

    ./configure CPPFLAGS=-I/my/path/include LDFLAGS=-L/my/path/lib

For best performance run `CXXFLAGS="-O3 -flto -march=native -mtune=native"
CXX="g++ -fno-semantic-interposition" ./configure` instead of `./configure` as
this code greatly benefits from flto inlining. (Unfortunately, libtool filters
out `-fno-semantic-interposition` as of early 2019 so we can not pass it as
part of `CXXFLAGS`. If you are using clang, `-fno-semantic-interposition` does
not seem to be necessary.) You might want to add `-g3` to the `CXXFLAGS`
which does not hurt performance but gives a better debugging experience. For
the best debugging experience, you might want to replace `-O3` with `-Og` or
even `-O0` but the latter results in very poor performance.

If your compiler supports it, you can try to add `-fvisibility=hidden
-fvisibility-inlines-hidden` to your `CXXFLAGS`. This hides internal bits in
the resulting library which have lead to crashes in the past due to conflicting
header-only libraries.

If your linker supports it, you should use `./configure --with-version-script`
to shrink the resulting shared library to an exact curated list of versioned
symbols.

Additionally, you might want to run configure with ` --disable-static` which
improves the build time.

[perf](https://perf.wiki.kernel.org/index.php/Main_Page) works well to profile
when you make sure that `CXXFLAGS` contains `-fno-omit-frame-pointer`. You can
then for example run our test suite with `perf record --call-graph dwarf make
check`. Apart from perf itself there are several ways to analyze the output,
[hotspot](https://github.com/KDAB/hotspot) might be the most convenient one at
the time of this writing.

## Installation

The preferred way to install intervalxt is to use your package manager (e.g.
`apt-get` on Debian or Ubuntu, `pacman` on Arch Linux, `brew` on MacOS, etc).
You can use [repology for
libintervalxt](https://repology.org/project/libintervalxt/packages) and for
[pyintervalxt](https://repology.org/project/pyintervalxt/packages) to see if
intervalxt is already available for your system.

Otherwise, you can build and install this package from source (as outlined
above) or install it with conda.
Download and install
[Mambaforge](https://github.com/conda-forge/miniforge#mambaforge), then run

```
mamba create -n intervalxt libintervalxt pyintervalxt
conda activate intervalxt
```

## Run with binder in the Cloud

You can try out the projects in this repository in a very limited environment
online by clicking the following links:

* **libintervalxt** [![Binder](https://mybinder.org/badge_logo.svg)](https://mybinder.org/v2/gh/flatsurf/intervalxt/3.3.0?filepath=binder%2FSample.libintervalxt.ipynb)
* **pyintervalxtc** [![Binder](https://mybinder.org/badge_logo.svg)](https://mybinder.org/v2/gh/flatsurf/intervalxt/3.2.0?filepath=binder%2FSample.pyintervalxt.ipynb)

## Build with conda-forge Dependencies

To build all of intervalxt, you need a fairly recent C++ compiler and probably
some packages that might not be readily available on your system. If you don't
want to use your distribution's packages, you can use these dependencies from
[conda-forge](https://conda-forge.org). Download and install
[Mambaforge](https://github.com/conda-forge/miniforge#mambaforge), then run

    mamba create -n intervalxt-build ccache
    mamba env update -n intervalxt-build -f libintervalxt/environment.yml
    mamba env update -n intervalxt-build -f pyintervalxt/environment.yml
    conda activate intervalxt-build
    export CC="ccache cc"  # to speed up future compilation
    export CXX="ccache c++"  # to speed up future compilation
    git clone --recurse-submodules https://github.com/flatsurf/intervalxt.git
    cd intervalxt
    ./bootstrap
    ./configure --prefix="$CONDA_PREFIX"
    make
    make check  # to run our test suite

## Run Tests and Benchmark

`make check` runs all tests and benchmarks. During development `make check
TESTS=module` only runs the tests for `module`. Note that the environment
variable `INTERVALXT_CHECK` is passed on to the tests and benchmarks, i.e., you
could add `INTERVALXT_CHECK="--benchmark_min_time=.02"` to not let the
benchmarks run as long as they would usually.

## How to Cite This Project

If you have used this project in the preparation of a publication, please cite
it as described [on our zenodo site](https://doi.org/10.5281/zenodo.4015050).

## Acknowledgements

* Julian Rüth's contributions to this project have been supported by the Simons
  Foundation Investigator grant of Alex Eskin.

## Maintainers

* [@saraedum](https://github.com/saraedum)
* [@videlec](https://github.com/videlec)
