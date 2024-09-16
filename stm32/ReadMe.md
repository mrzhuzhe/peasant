# Cross compile for libopencm3 

"""

export PREFIX_ARM32="$HOME/opt/cross-arm"
export TARGET_ARM32=arm-none-eabi
export PATH="$PREFIX_ARM32/bin:$PATH"

mkdir gcc-build-arm32
cd gcc-build-arm32
../gcc-14.1.0/configure --target=$TARGET_ARM32 --prefix="$PREFIX_ARM32" --disable-nls --enable-languages=c,c++ --without-headers

make all-gcc
make all-target-libgcc

make install-gcc
make install-target-libgcc

"""