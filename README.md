# Coin3D-SoQt-Examples

<!-- TOC depthFrom:1 depthTo:6 withLinks:1 updateOnSave:1 orderedList:0 -->

- [Coin3D-SoQt-Examples](#coin3d-soqt-examples)
	- [Installation](#installation)
		- [Install the Qt framework](#install-the-qt-framework)
		- [Install all the other needed external dependencies](#install-all-the-other-needed-external-dependencies)
			- [Install Simage](#install-simage)
			- [Install Coin, the 3D graphics engine](#install-coin-the-3d-graphics-engine)
			- [Install SoQt, the windowing glue package](#install-soqt-the-windowing-glue-package)
	- [Compiling the examples](#compiling-the-examples)
	- [Extra stuff](#extra-stuff)
		- [Testing with GCC7 on LXPLUS](#testing-with-gcc7-on-lxplus)

<!-- /TOC -->

## Installation

### Install the Qt framework

First of all, you should install the [Qt software framework](https://www1.qt.io/developers/) on your system.

Please, refer to the [official documentation](https://www.qt.io/download) for that. *(Please notice: there is an Open Source version  on the Installation page of Qt, you probably want to install that one)*

### Install all the other needed external dependencies

We are going to install all the external dependencies **locally**, i.e. _you do not need any administrator right_ to install and run this.

#### Install Simage

[Simage](https://bitbucket.org/Coin3D/simage) is the image manipulation library within the [Coin3D suite](https://bitbucket.org/Coin3D/).

1. Get Simage code:

```
hg clone https://bitbucket.org/Coin3D/simage
```

2. Now, add the line below to the `CMakeLists.txt` file, needed for modern versions of CMake (I will push the edit to the official Simage repo, later). *It is not really needed for us, actually, but you will get a warning from CMake if you don't have this variable set.*

```
set(CMAKE_MACOSX_RPATH 1)
```

3. Configure and compile the code:

```
mkdir build_simage
cd build_simage
cmake -DCMAKE_INSTALL_PREFIX=../install ../simage
make
make install
cd ..
```

*Note:* You might need of this setting before compiling the package, on certain operating systems (for example, on SLC):

```
export LD_LIBRARY_PATH=/usr/lib64:$LD_LIBRARY_PATH
```

#### Install Coin, the 3D graphics engine

1. Get the [Coin](https://bitbucket.org/Coin3D/coin/wiki/Home) source code, from the [CMake branch](https://bitbucket.org/Coin3D/coin/branch/CMake):

```
hg clone https://bitbucket.org/Coin3D/coin -r CMake
```

2. Get a couple of extra subrepositories, needed to build Coin:

```
hg clone https://bitbucket.org/Coin3D/boost-header-libs-full
hg clone https://bitbucket.org/Coin3D/generalmsvcgeneration
```

3. Create a build directory:

```
mkdir build_coin
cd build_coin
```

4. Configure, compile and install:

```
cmake -DCMAKE_INSTALL_PREFIX=../install ../coin
make
make install
cd ..
```

#### Install SoQt, the windowing glue package

[SoQt](https://bitbucket.org/Coin3D/soqt) links Coin with the Qt user interface. It allows to have OpenGL-based 3D content created with Coin displayed in a Qt widget.

1. Get the SoQt code

   * If you are on Linux, get the code from the official repository

   ```
   hg clone https://bitbucket.org/Coin3D/soqt
   ```
   
   * If you are on macOS, please get the code from the fork of , who fixed the problem with the Mac Retina display (there is a pull request pending, so in the future this fix will be integrated into the official SoQt repository):

```
hg clone https://bitbucket.org/roboticslibrary/soqt -r patch-hidpi
```

2. Now, like with Simage above, please add this line to the `CMakeLists.txt` file, needed for modern versions of CMake (I will push the edit to the official SoQt repo, later):

```
set(CMAKE_MACOSX_RPATH 1)
```

3. Configure and compile the code:

```
mkdir build_soqt
cd build_soqt
cmake -DCMAKE_INSTALL_PREFIX=../install ../soqt
makemake install
```

## Compiling the examples

Now, you can compile the examples. In general, you will be able to compile them by using the same `install` folder we used to install the external dependencies.

For example, taking the 'anti-aliasing' code as an example:

```
mkdir build_ex1_aa
cd build_ex1_aa
cmake -DCMAKE_INSTALL_PREFIX=../install ../Coin3D-SoQt-Examples/boilerplate_plus_antialiasing/
make
```

After the compilation, you will find an executable file inside the `build` folder.


---

## Extra stuff

### Testing with GCC7 on LXPLUS

If you work on CERN projects, you have access to the CERN computing network, and you want to test your code or the externals' code with Gcc7 on an Lxplus machine, here below you can find the relevant instructions. For convenience, we use the software framework of the ATLAS experiment to set Gcc7.

```
mkdir build_gcc7
cd build_gcc7
setupATLAS
asetup none,gcc7 --cmakesetup
cmake -DCMAKE_INSTALL_PREFIX=../install_gcc7 ../coin
make
make install
```
