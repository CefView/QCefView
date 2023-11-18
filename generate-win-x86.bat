cmake -S . ^
-B .build/windows.x86 ^
-A Win32 ^
-DPROJECT_ARCH=x86 ^
-DBUILD_DEMO=ON ^
-DCMAKE_INSTALL_PREFIX:PATH="%cd%/out/windows.x86" ^
%*