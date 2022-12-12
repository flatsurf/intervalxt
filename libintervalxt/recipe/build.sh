export CXXFLAGS="$CXXFLAGS -g3 -UNDEBUG"

cd libintervalxt

autoreconf -ivf

./configure --prefix="$PREFIX" --without-benchmark --without-eantic --without-exactreal --disable-static
make install
