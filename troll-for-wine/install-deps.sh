apt install gcc-multilib    # Needed for 32-bit builds
apt install gcc-mingw-w64 # PE format DLLs
apt install libasound2-dev # Sound backend
apt install libpulse-dev #  is a sound server for POSIX and WIN32 systems
apt install libdbus-1-dev # Dynamic device detection (specifically, mass storage)
apt install libfontconfig-dev # Host font enumeration
apt install libfreetype-dev # FreeType font reading
apt install libgnutls28-dev # Cryptography
apt install libgl-dev # you 3
apt install libunwind-dev # Necessary for x86_64 and arm64, but not used on other platforms.
apt install libx11-dev # Window management
apt install libxcomposite-dev # Window drawing
apt install libxcursor-dev  # Cursor management
apt install libxi-dev   # Keyboard and mouse
apt install libxrandr-dev # Display device management
apt install libxrender-dev # Window drawing
apt install libxext-dev # Window drawing
apt install libwayland-bin  # Only used by the Wayland driver as a build-time dependency.
apt install libwayland-dev  # Only used by the Wayland driver.
apt install libegl-dev  # OpenGL window support
apt install libwayland-dev  # OpenGL window support
apt install libxkbcommon-dev    # Keyboard support
apt install libxkbregistry-dev  # Keyboard support

apt install libgstreamer1.0-dev 
apt install libgstreamer-plugins-base1.0-dev

apt install libosmesa6-dev  # OpenGL bitmap support
apt install libsdl2-dev # Generally necessary for joystick or other HID support. Only one library is necessary, but they may exhibit different behaviour.
apt install libudev-dev # systemd
apt install libvulkan-dev   


../configure --enable-win64 CFLAGS="-O3" CROSSCFLAGS="-O3"
PKG_CONFIG_PATH=/usr/lib32 ../configure --with-wine64=../wine64-build CFLAGS="-O3" CROSSCFLAGS="-O3"
make -j12


# but wine seem to detect GPU wrong 