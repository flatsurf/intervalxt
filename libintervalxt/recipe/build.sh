autoreconf -ivf
./configure --prefix="$PREFIX" --without-benchmark
make install
