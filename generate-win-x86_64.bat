cmake -S . ^
-B .build/windows.x86_64 ^
-A x64 ^
-DPROJECT_ARCH=x86_64 ^
-DBUILD_DEMO=ON ^
-DCMAKE_INSTALL_PREFIX:PATH="%cd%/out/windows.x86_64" ^
%*