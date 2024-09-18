
# Linux build logs

![](linux-exit-code.svg)

```
system        : Linux
version       : #1 SMP Thu Oct 5 21:02:42 UTC 2023
platform      : Linux-5.15.133.1-microsoft-standard-WSL2-x86_64-with-glibc2.31
processor     : 
min frequency : 0.0 MHz
max frequency : 0.0 MHz
cores         : 4
```

build ran for `14.42 s` and terminated with exit code `0`

---

```bash
function PREREQUISITES {
    sudo apt -y update
    sudo apt install -y gcc cmake make libc6-dev
}

PREREQUISITES > .shci/linux/prerequisites-output.txt

```

```bash
Hit:1 http://deb.debian.org/debian bullseye InRelease
Get:2 http://deb.debian.org/debian bullseye-updates InRelease [44.1 kB]
Get:3 http://security.debian.org/debian-security bullseye-security InRelease [48.4 kB]
Get:4 http://ftp.debian.org/debian bullseye-backports InRelease [49.0 kB]
Get:5 http://deb.debian.org/debian bullseye-updates/main amd64 Packages.diff/Index [26.3 kB]
Get:6 http://deb.debian.org/debian bullseye-updates/main Translation-en.diff/Index [11.7 kB]
Get:7 http://deb.debian.org/debian bullseye-updates/main amd64 Packages T-2023-12-29-1403.39-F-2023-12-29-1403.39.pdiff [1,716 B]
Get:7 http://deb.debian.org/debian bullseye-updates/main amd64 Packages T-2023-12-29-1403.39-F-2023-12-29-1403.39.pdiff [1,716 B]
Get:8 http://security.debian.org/debian-security bullseye-security/main amd64 Packages [265 kB]
Get:9 http://deb.debian.org/debian bullseye-updates/main Translation-en T-2023-12-29-1403.39-F-2023-12-29-1403.39.pdiff [839 B]
Get:9 http://deb.debian.org/debian bullseye-updates/main Translation-en T-2023-12-29-1403.39-F-2023-12-29-1403.39.pdiff [839 B]
Get:10 http://security.debian.org/debian-security bullseye-security/main Translation-en [171 kB]
Get:11 http://ftp.debian.org/debian bullseye-backports/main amd64 Packages.diff/Index [63.3 kB]
Get:12 http://ftp.debian.org/debian bullseye-backports/main amd64 Packages T-2024-01-03-2005.27-F-2024-01-03-2005.27.pdiff [1,741 B]
Get:12 http://ftp.debian.org/debian bullseye-backports/main amd64 Packages T-2024-01-03-2005.27-F-2024-01-03-2005.27.pdiff [1,741 B]
Fetched 683 kB in 1s (561 kB/s)
Reading package lists...
Building dependency tree...
Reading state information...
39 packages can be upgraded. Run 'apt list --upgradable' to see them.
Reading package lists...
Building dependency tree...
Reading state information...
cmake is already the newest version (3.18.4-2+deb11u1).
gcc is already the newest version (4:10.2.1-1).
make is already the newest version (4.3-4.1).
libc6-dev is already the newest version (2.31-13+deb11u7).
0 upgraded, 0 newly installed, 0 to remove and 39 not upgraded.

```

---
    
```bash
function BUILD {

    mkdir build
    cd    build
	mkdir linux
	cd    linux

	cmake ../../ -DSH_TEMPLATE_BUILD_EXAMPLE=ON
	cmake --build .
}

BUILD > .shci/linux/build-output.txt

```

```bash
-- The C compiler identification is GNU 10.2.1
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /usr/bin/cc - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- SH_TEMPLATE_VERSION       : pre-release
-- SH_TEMPLATE_ROOT_DIR      : /mnt/d/home/desktop/github/shtemplate
-- SH_TEMPLATE_BINARIES_DIR  : /mnt/d/home/desktop/github/shtemplate/bin
-- SH_TEMPLATE_BUILD_EXAMPLE : ON
-- Could NOT find Doxygen (missing: DOXYGEN_EXECUTABLE) 
-- Configuring done
-- Generating done
-- Build files have been written to: /mnt/d/home/desktop/github/shtemplate/build/linux
Scanning dependencies of target shtemplate
[ 25%] Building C object CMakeFiles/shtemplate.dir/shtemplate/src/shtemplate.c.o
[ 50%] Linking C static library ../../bin/linux/libshtemplate.a
[ 50%] Built target shtemplate
Scanning dependencies of target shtemplate-example
[ 75%] Building C object CMakeFiles/shtemplate-example.dir/example/src/shtemplate-example.c.o
[100%] Linking C executable ../../bin/linux/shtemplate-example
[100%] Built target shtemplate-example

```

---
    
build ran for `14.42 s` and terminated with exit code `0`

---

