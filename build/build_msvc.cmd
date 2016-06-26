if defined VS110COMNTOOLS set MSVCDIR=%VS110COMNTOOLS%
if defined VS120COMNTOOLS SET XPTARGET=-DCMAKE_VS_PLATFORM_TOOLSET=v110_xp
if defined VS110COMNTOOLS set GENERAT="Visual Studio 11 2012"

if defined VS120COMNTOOLS set MSVCDIR=%VS120COMNTOOLS%
if defined VS120COMNTOOLS SET XPTARGET=-DCMAKE_VS_PLATFORM_TOOLSET=v120_xp
if defined VS120COMNTOOLS set GENERAT="Visual Studio 12 2013"

if defined VS140COMNTOOLS set MSVCDIR=%VS140COMNTOOLS%
if defined VS120COMNTOOLS SET XPTARGET=-DCMAKE_VS_PLATFORM_TOOLSET=v140_xp
if defined VS140COMNTOOLS set GENERAT="Visual Studio 14 2015"

call "%MSVCDIR%\vsvars32.bat"

rmdir /S /Q build_msvc
mkdir build_msvc
cd build_msvc
cmake -G %GENERAT% %XPTARGET% ../..
cmake --build . --config Release
cmake --build . --config Release --target install
cd ..
