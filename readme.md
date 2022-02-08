libevent-2.1.12
fmt
mkdir build
cd build
MacOS:
cmake  -DOPENSSL_ROOT_DIR=/usr/local/opt/openssl -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_STANDARD=17 ..
Others:
cmake  -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_STANDARD=17 ..