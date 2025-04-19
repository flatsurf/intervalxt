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

## Build and Develop intervalxt with pixi

If you have cloned the source repository, make sure to pull in all the
third-party header-only libraries by running:

```
git submodule update --init
```

If you are distributing intervalxt or you are a purist who *only* wants to
interact with autotools directly, then you should skip to the [next
section](#build-from-the-source-code-repository-or-a-tarball). Otherwise, we
strongly recommend that you install [pixi](https://pixi.sh) and then use the
following commands:

* `pixi run test` build intervalxt and run the libintervalxt and pyintervalxt test suites
* `pixi run compile-commands` to generate a `compile_commands.json` that your IDE might be able to use to make sense of this project

<details>
<summary>What is pixi?</summary>

pixi is a tool based on
[conda](https://en.wikipedia.org/wiki/Conda_(package_manager)) &
[conda-forge](https://conda-forge.org) for developers so that we can all use
the same workflows in the same defined environments.

pixi allows us to ship a very opinionated setup to developers of intervalxt
namely a number of opinionated scripts with corresponding tested (and
opinionated) dependencies.

This makes the whole development experience much more reliable and
reproducible, e.g., the CI on GitHub Pull Requests runs with the exact same
setup, so if something fails there, you can just run the CI command to
hopefully get exactly the same behavior locally.
</details>

<details>
<summary>How do I use pixi?</summary>

If you have not used pixi before, the most relevant pixi command is:

```sh
pixi run TASK
```

Run `pixi task list` to see the available tasks.

All tasks are defined in the `pixi.toml` file and most are used somewhere in
our GitHub Continuous Integration setup, see .github/workflows/.
</details>

<details>
<summary>Why don't we add all things to the Makefiles but use pixi tasks?</summary>

Packagers do prefer a system that is as minimalistic as possible. Any
opinionated bit in the build system, such as setting compiler flags, usually
needs to be patched out by software distributions. That's why our Makefiles are
trying to follow the autoconfiscated standards as closely as possible. And
essentially all that pixi does is to call these Makefiles without you having to
figure out how everything works in detail.
</details>

<details>
<summary>Can I use configure & make with pixi?</summary>

More experienced developers may not want to use these tasks. You can also just
use the curated list of dependencies that pixi provides and drop into a shell
with these dependencies installed. For example, to run the libintervalxt test suite
directly, you could do:

```sh
pixi shell -e dev
./bootstrap
cd libintervalxt
./configure
make check
```

Note that the following section contains more details about this `configure &&
make` workflow that might be of interest to you.
</details>

## Build from the Source Code Repository or a Tarball

If you have cloned the source directory and you decided not to use pixi, you
will need to setup the configure script and Makefile using autotools. That is

    git submodule update --init
    ./bootstrap

If you obtained a tarball of the sources or if the preceding step
worked, you can now run

    ./configure
    make
    make check  # to run our test suite
    make install  # to install into /usr/local

If you happen to have any of FLINT, Arb, or ANTIC installed in a non standard
directory, you will have to specify the `CPPFLAGS` and `LDFLAGS` variables for
the configure script

    ./configure CPPFLAGS=-I/my/path/include LDFLAGS=-L/my/path/lib

For best performance run `CFLAGS="-O3" CXXFLAGS="-O3" ./configure` instead of
`./configure`.  You might want to add `-g3` to `CFLAGS` and `CXXFLAGS` which
does not hurt performance but gives a better debugging experience. For the best
debugging experience, you might want to replace `-O3` with `-Og` or even `-O0`
but the latter results in poor performance.

If your compiler supports it, you can try to add `-fvisibility=hidden
-fvisibility-inlines-hidden` to your `CXXFLAGS`. This hides internal bits in
the resulting library which have lead to crashes in the past due to conflicting
header-only libraries.

If your linker supports it, you should use `./configure --with-version-script`
to shrink the resulting shared library to an exact curated list of versioned
symbols.

perf works well to profile when you make sure that `CFLAGS` and `CXXFLAGS`
contain `-fno-omit-framepointer`. You can then for example run our test suite
with `perf record --call-graph dwarf make check`. Apart from perf itself there
are several ways to analyze the output,
[hotspot](https://github.com/KDAB/hotspot) might be the most convenient one at
the time of this writing.

For more detailed but generic instructions please refer to the INSTALL file.

## How to Cite This Project

If you have used this project in the preparation of a publication, please cite
it as described [on our zenodo site](https://doi.org/10.5281/zenodo.4015050).

## Acknowledgements

* Julian Rüth's contributions to this project have been supported by the Simons
  Foundation Investigator grant of Alex Eskin.

## Maintainers

* [@saraedum](https://github.com/saraedum)
* [@videlec](https://github.com/videlec)
