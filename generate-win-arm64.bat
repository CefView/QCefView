cmake -S . ^
-B .build/windows.arm64 ^
-A ARM64 ^
-DPROJECT_ARCH=arm64 ^
-DBUILD_DEMO=ON ^
-DCMAKE_INSTALL_PREFIX:PATH="%cd%/out/windows.arm64" ^
%*