
# Windows build logs

![](windows-exit-code.svg)

```
system        : Windows
version       : 10.0.22621
platform      : Windows-10-10.0.22621-SP0
processor     : AMD64 Family 23 Model 17 Stepping 0, AuthenticAMD
min frequency : 0.0 MHz
max frequency : 3500.0 MHz
cores         : 4
```

build ran for `46.54 s` and terminated with exit code `0`

---

```bash
> .shci/windows/prerequisites-output.txt (

    choco install -y mingw cmake
    mingw-get install gcc mingw32-make gdb

    #echo installing build tools
    #echo creating C:/bin directory
    #mkdir C:/bin
    #
    #echo installing MinGW and related packages
    #wget https://github.com/MrSinho/mingw/releases/download/0.6.2-beta-20131004-1/mingw.zip -o C:/bin/mingw.zip
    #cd C:/bin/
    #tar -xf C:/bin/mingw.zip
    #del C:/bin/mingw.zip
    #./C:/bin/mingw/bin/mingw-get.exe install gcc g++ mingw32-make gdb
    #
    #echo installing CMake
    #wget https://github.com/Kitware/CMake/releases/download/v3.28.0-rc2/cmake-3.28.0-rc2-windows-x86_64.zip -o C:/bin/cmake.zip
    #cd C:/bin
    #tar -xf C:/bin/cmake.zip
    #mv C:/bin/cmake-3.28.0-rc2-windows-x86_64 C:/bin/cmake
    #del C:/bin/cmake.zip
    #
    #echo installing Doxygen
    #wget https://github.com/doxygen/doxygen/releases/download/Release_1_9_8/doxygen-1.9.8.windows.x64.bin.zip -o C:/bin/doxygen.zip
    #mkdir C:/bin/doxygen
    #mkdir C:/bin/doxygen/bin
    #cd C:/bin/doxygen/bin
    #tar -xf C:/bin/doxygen.zip
    #del C:/bin/doxygen.zip
    #
    #echo installing GNU Arm Toolchain
    #wget https://developer.arm.com/-/media/Files/downloads/gnu/12.3.rel1/binrel/arm-gnu-toolchain-12.3.rel1-mingw-w64-i686-arm-none-eabi.zip -o C:/bin/gnu-arm-toolchain.zip
    #cd C:/bin
    #tar -xf C:/bin/gnu-arm-toolchain.zip
    #mv C:/bin/arm-gnu-toolchain-12.3.rel1-mingw-w64-i686-arm-none-eabi C:/bin/gnu-arm-toolchain
    #del C:/bin/gnu-arm-toolchain.zip
    #
    #echo installing vulkan-sdk
    #wget https://github.com/MrSinho/vulkan-sdk/releases/download/1.3.261.1/vulkan-sdk-1.3.261.1.zip -o C:/lib/vulkan-sdk.zip
    #cd C:/lib
    #tar -xf C:/lib/vulkan-sdk.zip
    #del C:/lib/vulkan-sdk.zip
    #setx /M VK_SDK_PATH C:/lib/vulkan-sdk-1.3.261.1
    #setx /M VULKAN_SDK C:/lib/vulkan-sdk-1.3.261.1
    #
    #echo installing pico-sdk
    #git clone --recursive https://github.com/raspberrypi/pico-sdk C:/lib/pico-sdk
    #setx /M PICO_SDK_PATH C:/lib/pico-sdk
    #
    #echo installing Python-3.12.0
    #wget https://www.python.org/ftp/python/3.12.0/python-3.12.0-embed-amd64.zip -o C:/bin/python-3.12.0.zip
    #mkdir C:/bin/python-3.12.0
    #cd C:/bin/python-3.12.0
    #tar -xf C:/bin/python-3.12.0.zip
    #copy C:/bin/python-3.12.0/python.exe C:/bin/python-3.12.0/python-3.12.0.exe 
    #del C:/bin/python-3.12.0.zip
    #
    #echo add C:/bin/mingw/bin to system Path
    #echo add C:/bin/cmake/bin to system Path
    #echo add C:/bin/doxygen/bin to system Path
    #echo add C:/bin/gnu-arm-toolchain/bin to system Path

)
```

```bash
Chocolatey v2.0.0
2 validations performed. 1 success(es), 1 warning(s), and 0 error(s).

Validation Warnings:
 - A pending system reboot request has been detected, however, this is
   being ignored due to the current Chocolatey configuration.  If you
   want to halt when this occurs, then either set the global feature
   using:
     choco feature enable -name=exitOnRebootDetected
   or pass the option --exit-when-reboot-detected.

Chocolatey detected you are not running from an elevated command shell
 (cmd/powershell).

 You may experience errors - many functions/packages
 require admin rights. Only advanced users should run choco w/out an
 elevated shell. When you open the command shell, you should ensure
 that you do so with "Run as Administrator" selected. If you are
 attempting to use Chocolatey in a non-administrator setting, you
 must select a different location other than the default install
 location. See
 https://docs.chocolatey.org/en-us/choco/setup#non-administrative-install
 for details.

For the question below, you have 20 seconds to make a selection.

 Do you want to continue?([Y]es/[N]o): 
Timeout or your choice of '' is not a valid selection.
Installing the following packages:
mingw;cmake
By installing, you accept licenses for the packages.
mingw v12.2.0.03042023 already installed.
 Use --force to reinstall, specify a version to install, or try upgrade.
cmake v3.26.4 already installed.
 Use --force to reinstall, specify a version to install, or try upgrade.

Chocolatey installed 0/2 packages. 
 See the log for details (C:\ProgramData\chocolatey\logs\chocolatey.log).

Warnings:
 - cmake - cmake v3.26.4 already installed.
 Use --force to reinstall, specify a version to install, or try upgrade.
 - mingw - mingw v12.2.0.03042023 already installed.
 Use --force to reinstall, specify a version to install, or try upgrade.

```

---
    
```bash
> .shci/windows/build-output.txt (
	
    mkdir build
    cd    build
	mkdir windows
	cd    windows

	cmake ../../ -DSH_TEMPLATE_BUILD_EXAMPLE=ON
	cmake --build .
)

```

```bash
-- Building for: Visual Studio 17 2022
-- Selecting Windows SDK version 10.0.22000.0 to target Windows 10.0.22621.
-- The C compiler identification is MSVC 19.37.32822.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: D:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.37.32822/bin/Hostx64/x64/cl.exe - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- SH_TEMPLATE_VERSION       : pre-release
-- SH_TEMPLATE_ROOT_DIR      : D:/home/desktop/github/shtemplate
-- SH_TEMPLATE_BINARIES_DIR  : D:/home/desktop/github/shtemplate/bin
-- SH_TEMPLATE_BUILD_EXAMPLE : ON
-- Found Doxygen: D:/bin/doxygen/bin/doxygen.exe (found version "1.9.8") found components: doxygen missing components: dot
-- Configuring D:/home/desktop/github/shtemplate/docs/Doxyfile from D:/home/desktop/github/shtemplate/docs/Doxyfile.in
-- Creating custom docs command: D:/bin/doxygen/bin/doxygen.exe D:/home/desktop/github/shtemplate/docs/Doxyfile
-- Configuring done (6.5s)
-- Generating done (0.1s)
-- Build files have been written to: D:/home/desktop/github/shtemplate/build/windows
Versione di MSBuild L17.7.2+d6990bcfa per .NET Framework

  Checking Build System
  Building Custom Rule D:/home/desktop/github/shtemplate/CMakeLists.txt
  shtemplate.c
  shtemplate.vcxproj -> D:\home\desktop\github\shtemplate\bin\windows\Debug\shtemplate.lib
  Building Custom Rule D:/home/desktop/github/shtemplate/CMakeLists.txt
  shtemplate-example.c
  shtemplate-example.vcxproj -> D:\home\desktop\github\shtemplate\bin\windows\Debug\shtemplate-example.exe
  Building Custom Rule D:/home/desktop/github/shtemplate/CMakeLists.txt

```

---
    
build ran for `46.54 s` and terminated with exit code `0`

---

