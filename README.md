# SDCC PIC12F617 example

## Requirement

1. [SDCC](https://sourceforge.net/projects/sdcc/files/)
2. [GPUTILS](https://sourceforge.net/projects/gputils/files/)
3. [CMake](https://github.com/Kitware/CMake/releases)
4. [Ninja](https://github.com/ninja-build/ninja/releases)

## How to Build

modify the `CMAKE_C_COMPILER` variable in `CMakeLists.txt`

```bash
$ cd build
$ cmake -G Ninja ..
$ ninja
```
