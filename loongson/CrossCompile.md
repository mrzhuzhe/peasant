# Cross compile for loognarch64
 
## Target
```
export PREFIX_LOONGSON="$HOME/opt/loongarch64"
export TARGET_LOONGSON=loongarch64-unknown-linux-gnu
export PATH="$PREFIX_LOONGSON/bin:$PATH"
```

## BINUTILS
```
mkdir ./binutils-build-loongarch64
cd  ./binutils-build-loongarch64

../binutils-2.43.1/configure --target=$TARGET_LOONGSON --prefix="$PREFIX_LOONGSON" --with-sysroot --disable-nls --disable-werror
make
make install

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


## GCC first time
```
mkdir ../gcc-build-loongarch64
cd ../gcc-build-loongarch64
../gcc-14.2.0/configure --target=$TARGET_LOONGSON --prefix="$PREFIX_LOONGSON" \
--with-newlib --disable-shared --with-sysroot\
		             --disable-decimal-float --disable-libgomp --disable-libitm \
		             --disable-libsanitizer --disable-libquadmath --disable-threads \
		             --disable-target-zlib --with-system-zlib --enable-checking=release \
		             --enable-default-pie \
		             --enable-languages=c

make all-gcc
make all-target-libgcc

make install-gcc
make install-target-libgcc

```
### Issue 
1. mawk error in gcc https://stackoverflow.com/questions/16921493/awk-illegal-reference-to-array-a