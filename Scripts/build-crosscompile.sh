#!/usr/bin/env bash

export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

# BINUTILS
wget https://ftp.gnu.org/gnu/binutils/binutils-2.39.tar.xz -P /tmp/osdevtoolchain
tar xf /tmp/osdevtoolchain/binutils-2.39.tar.xz -C /tmp/osdevtoolchain
mkdir $HOME/src
cd $HOME/src
mkdir build-binutils
cd build-binutils
/tmp/osdevtoolchain/binutils-2.39/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make -j4
make install -j4

# GCC
wget https://ftp.gnu.org/gnu/gcc/gcc-11.2.0/gcc-11.2.0.tar.xz -P /tmp/osdevtoolchain
tar xf /tmp/osdevtoolchain/gcc-11.2.0.tar.xz -C /tmp/osdevtoolchain
mkdir $HOME/src
cd $HOME/src
# The $PREFIX/bin dir _must_ be in the PATH. We did that above.
which -- $TARGET-as || echo $TARGET-as is not in the PATH
mkdir build-gcc
cd build-gcc
/tmp/osdevtoolchain/gcc-11.2.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
make all-gcc -j4
make all-target-libgcc -j4
make install-gcc -j4
make install-target-libgcc -j4
