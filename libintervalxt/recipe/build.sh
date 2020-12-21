export CXXFLAGS="$CXXFLAGS -g3"

autoreconf -ivf

./configure --prefix="$PREFIX" --without-benchmark --without-eantic --without-exactreal
make install
