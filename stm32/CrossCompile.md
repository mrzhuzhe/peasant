# Cross compile for arm cortex-M3 used in stm32
 
## Target
```
export PREFIX_ARM32="$HOME/opt/cross-arm"
export TARGET_ARM32=arm-none-eabi
export PATH="$PREFIX_ARM32/bin:$PATH"
```

## BINUTILS
```
mkdir ./binutils-build-arm32
cd  ./binutils-build-arm32

../binutils-2.43/configure --target=$TARGET_ARM32 --prefix="$PREFIX_ARM32" --with-sysroot --disable-nls --disable-werror
make
make install

```

## GDB

```
mkdir ../gdb-build-arm32
cd  ../gdb-build-arm32
../gdb-15.2/configure --target=$TARGET_ARM32 --prefix="$PREFIX_ARM32" --disable-werror
make all-gdb
make install-gdb

```


## GCC prerequire gmp mpfr and mpc
```
cd ../gmp-6.3.0
./configure
make 
make install 

cd ../mpfr-4.2.1
./configure
make 
make install 

cd ../mpc-1.2.1
./configure
make 
make install 
# notice there is a bug with mpc that gcc cannot find global mpc in mpc-1.3.0

```


## GCC
```
mkdir ../gcc-build-arm32
cd ../gcc-build-arm32
../gcc-14.2.0/configure --target=$TARGET_ARM32 --prefix="$PREFIX_ARM32" --disable-nls --enable-languages=c,c++ --without-headers

make all-gcc
make all-target-libgcc

make install-gcc
make install-target-libgcc

```

# Setup libopencm3.a

## NewLib

```

cd ../newlib-cygwin-cygwin-3.5.4/
mkdir ../newlib-arm32-aout
cd ../newlib-arm32-aout
../newlib-cygwin-cygwin-3.5.4/configure --target=$TARGET_ARM32 --prefix="$PREFIX_ARM32"
make
make install

```

## libopencm3 

```
# only need make f1
make TARGETS='stm32/f1'

```

## stlink

```
apt-get install libusb-1.0
make release 
make install
```