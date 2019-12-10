# Drone Simulator
Drone battle simulator written in C++.

### Installation
- Dependencies are managed through Conan. If you already have Python installed you can simply do `pip install conan`.
- OpenCL ICD has a dependency on the [Windows Driver Kit](https://docs.microsoft.com/en-us/windows-hardware/drivers/download-the-wdk) when compiling for Windows. Conan will fail to build OpenCL ICD on Windows if you do not have it installed.
- OpenCL ICD just links to an existing OpenCL implementation on your system. You still need to have an OpenCL implementation installed.
- If above requirements are met, you can simply run `./dependencies/install.bat` if using Windows with Visual Studio. If not, edit the generator in `conanfile.txt` and run conan manually. (Look in `install.bat` for the arguments used.)

### Adding Resources
Resources, like OpenCL kernels, should be added to the `./copy_to_build/` folder.
Before building, `./onPrebuild.bat` should be run to copy them to the build directory. When using Visual Studio, this will be done automatically.