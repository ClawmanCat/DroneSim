@echo off


if not exist "%~dp0\debug\" mkdir %~dp0\debug 
if not exist "%~dp0\release\" mkdir %~dp0\release


echo Running conan for debug configuration...

set CONAN_TRACE_FILE=%~dp0debug\conan_trace.log
type nul > %CONAN_TRACE_FILE%

:: Change --build missing to --build if you need PDB files. This will take significantly longer.
call conan install . --profile ./debug.conanprofile --install-folder "%~dp0/debug/" --build missing


echo Running conan for release configuration...

set CONAN_TRACE_FILE=%~dp0release\conan_trace.log
type nul > %CONAN_TRACE_FILE%

:: GLEW and OpenCL ICD don't build with VS2019, so use VS2017 instead.
:: For GLEW, this is fixed in GLEW 2.2.0, but there is no Conan package for that version yet.
call conan install . --profile ./release.conanprofile --install-folder "%~dp0/release/" --build missing ^
     -s glew:compiler="Visual Studio" -s glew:compiler.version=15 -s glew:compiler.toolset=v141 ^
     -s khronos-opencl-icd-loader:compiler="Visual Studio" -s khronos-opencl-icd-loader:compiler.version=15 -s khronos-opencl-icd-loader:compiler.toolset=v141