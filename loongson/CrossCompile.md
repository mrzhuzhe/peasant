# Cross compile for loognarch64
 
## Target
```
export PREFIX_LOONGSON="$HOME/opt/loongarch64"
export TARGET_LOONGSON=loongarch64-unknown-linux-gnu
export PATH="$PREFIX_LOONGSON/bin:$PATH"
```

## Headers
```
	cd linux 
	git checkout v6.13-rc7
	make mrproper
	make ARCH=loongarch INSTALL_HDR_PATH=dest headers_install
	find dest/include -name '.*' -delete
	mkdir -pv ${PREFIX_LOONGSON}/sysroot/usr/include
	cp -rv dest/include/* ${PREFIX_LOONGSON}/sysroot/usr/include

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
1. mawk error in gcc https://stackoverflow.com/questions/16921493/awk-illegal-reference-to-array-a need gawk
2. --build=x86_64-cross-linux-gnu 
3. export LD_LIBRARY_PATH="/usr/local/cuda/lib64" tempory disable current path ld path

## libc
```

mkdir ../glibc-build-loongarch64
cd ../glibc-build-loongarch64



BUILD_CC="gcc" CC="${TARGET_LOONGSON}-gcc -mabi=lp64d" \
CXX="${TARGET_LOONGSON}-gcc -mabi=lp64d" \
AR="${TARGET_LOONGSON}-ar" RANLIB="${TARGET_LOONGSON}-ranlib" \
../glibc-2.40/configure --prefix=/usr --host=${TARGET_LOONGSON} \
				--libdir=/usr/lib64 --libexecdir=/usr/lib64/glibc \
				--with-binutils=${PREFIX_LOONGSON}/bin \
				--with-headers=${PREFIX_LOONGSON}/sysroot/usr/include \
				--enable-stack-protector=strong --enable-add-ons \
				--disable-werror --disable-nscd libc_cv_slibdir=/usr/lib64 
make -j12
make DESTDIR=${PREFIX_LOONGSON}/sysroot install

```

