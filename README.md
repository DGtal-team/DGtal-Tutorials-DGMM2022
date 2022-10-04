# DGtal-Tutorials-DGMM2022

## Get the code

To get the code, the easiest way is to `clone` the github repository. E.g. with the command-line git client:

    git clone https://github.com/DGtal-team/DGtal-Tutorials-DGMM2022.git

(You can also use your favorite GUI git client).

## First build

To build DGtal related examples, you would need:
  - C++11 enabled compiler (most c++ compilers are C+11)
  - a [cmake](https://cmake.org) client (at least 3.13)
  - [boost](http://boost.org) (>= 1.50).
  - the zlib package (already installed in many linux/macos distributions)

From cmake, you can create the DGtal-DGMM2022 tutorial project, either from the commandline, or from a cmake GUI. E.g., from the commandline (using a Makefile target):

```
mkdir build
cd build
cmake ..
make
```

(`cmake .. -G Xcode` for an Xcode project, or `cmake .. -G "CodeBlocks - Unix Makefiles"` for a codeblocks one, or VisualStudio on MS Windows using the GUI)


By default, `cmake` will clone a copy of the DGtal repository, set up all the dependencies and build a first `helloworld` program.

## The tutorials
