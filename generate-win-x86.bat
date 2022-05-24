@rem OSR mode is enabled by default, add -DUSE_OSR=OFF to disable the OSR mode
cmake -S . ^
-B .build/windows.x86 ^
-A Win32 ^
-DPROJECT_ARCH=x86 ^
-DBUILD_DEMO=ON ^
-DCMAKE_INSTALL_PREFIX:PATH="%cd%/out/install/windows.x86"