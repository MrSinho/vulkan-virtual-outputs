# Vulkan Virtual Outputs

A lightweight library for capturing and streaming Vulkan images in a local http server, directly from the application mainloop. This projects may be used for remote devices and terminal-like environments, which hardware does support graphics operations, but do not support Desktop like windows (e.g. Docker containers, terminal virtual machines, embedded Linux devices, servers, etc.).   

![](https://img.shields.io/badge/Sinho_softworks%20|%20Vulkan%20Virtual%20Outputs-FFBF00?style=for-the-badge&logo=&logoColor=white&labelColor=990042)
[![](https://img.shields.io/badge/GitHub_repository-000000?style=for-the-badge&logo=github&logoColor=white)](https://github.com/mrsinho/shtemplate)

[TOC]

![](./docs/media/gradient.png)
![](./docs/media/headless-scene-browser.png)


## Overview

![](.shci/linux/linux-exit-code.svg)
![](.shci/windows/windows-exit-code.svg)

![](https://img.shields.io/badge/Written_in_C-FFBF00?style=for-the-badge&logo=c&logoColor=white&labelColor=FFA000#.svg)
![](https://img.shields.io/badge/Compatible_with_C%2b%2b-FFBF00?style=for-the-badge&logo=c%2b%2b&logoColor=white&labelColor=FFA000#.svg)

## Build with cmake

Building the library is relatively simple:

```shell
#starting from root directory
mkdir build
cd    build
cmake .. -DVVO_BUILD_EXAMPLE=ON
cmake --build .
```

## Copy JS files

Before running the example, be sure the `js` folder is in the same directory of the example binaries:

```shell
#starting from root directory
cp -r vvo/js bin/windows/js
cp -r vvo/js bin/linux/js
```

## Embed the library into your C/C++ Vulkan projects

First embed the library with your `CMakeLists.txt` or `.cmake` file:

```cmake
if (NOT TARGET vvo)
set(VVO_BINARIES_DIR path/to/your/binaries/directory)
set(VVO_ROOT_DIR     path/to/vvo/root/directory)
include(${VVO_ROOT_DIR}/vvo/vvo.cmake)
build_vvo()
endif()

# [...]

target_link_libraries(app PUBLIC vvo)

```

For more details about embedding `vvo` into your C/C++ application, see the [`shvulkan headless scene`](https://github.com/mrsinho/shvulkan/tree/main/examples/src/graphics/headless-scene.c) example, which replicates the native desktop example named [`shvulkan-scene`](https://github.com/mrsinho/shvulkan/tree/main/examples/src/graphics/scene.c).

## Supported URIs

* `/static-image`: displays a single png image capture;
* `/vvoStream`: displays multiple frames of the Vulkan applications being captured almost in real time (performance issues need to be fixed). 

## Contributions and support

Should you encounter any issues with the tools or wish to introduce new features, I welcome your contributions to the repository. You can contribute by either opening an issue or submitting a pull request.


[![](https://img.shields.io/badge/Buy_Me_A_Coffee-FFDD00?style=for-the-badge&logo=buy-me-a-coffee&logoColor=black)](https://www.buymeacoffee.com/mrsinho)
![](https://img.shields.io/badge/Sinho_softworks%20|%20Vulkan%20Virtual%20Outputs-FFBF00?style=for-the-badge&logo=&logoColor=white&labelColor=990042)
