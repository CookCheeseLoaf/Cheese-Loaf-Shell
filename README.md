# Welcome to the Cheese Loaf Shell (cls)
A portable minimalist heavily Influnced by DOS shell and command prompt shell.

> WARNING: The shell is on ALPHA release, not all command works out of the box,
> **Use it in your own risk!**.
> ~~I don't want to come around and asking me what happened to your os. lol~~

You are welcome to help if you want. :-)

---
### How to build it?
the shell is written in C++20, so you need a compiler that support C++17 directly.

Before you could build it, you would **obviously** need dependencies.
+ Compiler (GCC/Clang/MSVC) Make sure it support C++17, otherwise it would refuse to build.
+ [Replxx](https://github.com/AmokHuginnsson/replxx) (static build `.a` required)

```cmake
cmake -S . -B build
```
```cmake
cmake --build build
```
> the **build** can be named to anything, build_temp, temp_build, it won't matter.

after building it, you could move the executable file into the binary directories where you can call its shell through your terminal.
```cmake
cmake --install build
```
> this step require adminstrator/root access, so use sudo, doas, or su in unix, and logged into adminstrator in windows would suffice.

---
### How to run it?
you can either run the clshell in the compiled **build** directories or call it directly in terminal (after moving it through the binary directories)
```shell
clshell
```
