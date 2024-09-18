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