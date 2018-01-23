# Coin3D-SoQt-Examples

<!-- TOC depthFrom:1 depthTo:6 withLinks:1 updateOnSave:1 orderedList:0 -->

- [Coin3D-SoQt-Examples](#coin3d-soqt-examples)
	- [Installation](#installation)
		- [Get the needed external dependencies](#get-the-needed-external-dependencies)
			- [Install Simage](#install-simage)
			- [Install Coin, the 3D graphics engine](#install-coin-the-3d-graphics-engine)
			- [Install SoQt, the windowing glue package](#install-soqt-the-windowing-glue-package)
	- [Extra stuff](#extra-stuff)
		- [Testing with GCC7 on LXPLUS](#testing-with-gcc7-on-lxplus)

<!-- /TOC -->

## Installation

### Get the needed external dependencies

#### Install Simage

[Simage](https://bitbucket.org/Coin3D/simage) is the image manipulation library within the [Coin3D suite](https://bitbucket.org/Coin3D/).

1. Get Simage code:

```
hg clone https://bitbucket.org/Coin3D/simage
```

2. Now, add this line to the `CMakeLists.txt` file, needed for modern versions of CMake (I will push the edit to the official Simage repo, later):

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

1. Get the SoQt code:

```
hg clone https://bitbucket.org/Coin3D/soqt
```

2. Now, like with Simage above, you need to add this line to the `CMakeLists.txt` file, needed for modern versions of CMake (I will push the edit to the official SoQt repo, later):

```
set(CMAKE_MACOSX_RPATH 1)
```

3. Then, you have to setup your Qt installation, if not setup on login by default.
A working and correctly configured Qt environment is needed to compile SoQt.

4. Configure and compile the code:

```
mkdir build_soqt
cd build_soqt
cmake -DCMAKE_INSTALL_PREFIX=../install ../soqt
makemake install
```

---

## Extra stuff

### Testing with GCC7 on LXPLUS

If you are at CERN and you want to test your code or the externals' code with Gcc7 on an Lxplus machine, here below are the instructions. We use the using the software framework of the ATLAS experiment to set Gcc7.

```
mkdir build_gcc7
cd build_gcc7
setupATLAS
asetup none,gcc7 --cmakesetup
cmake -DCMAKE_INSTALL_PREFIX=../install_gcc7 ../coin
make
make install
```
