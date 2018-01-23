# Coin3D-SoQt-Examples

## Installation

### Get the needed external dependencies

#### Install Simage

[Simage](https://bitbucket.org/Coin3D/simage) is the image manipulation library within the [Coin3D suite](https://bitbucket.org/Coin3D/).

Get Simage code and compile it:

```
hg clone https://bitbucket.org/Coin3D/simage
mkdir build_simage
cd build_simage
setupATLAS
asetup none,gcc7 --cmakesetup
lsetup python
cmake -DCMAKE_INSTALL_PREFIX=../install ../simage
export LD_LIBRARY_PATH=/usr/lib64:$LD_LIBRARY_PATH # if you need it
make
make install
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
mkdir build
cd build
```

4. Compile and install:

```
cmake -DCMAKE_INSTALL_PREFIX=../install ../coin
make
make install
```

#### Install SoQt, the windowing glue package

[SoQt](https://bitbucket.org/Coin3D/soqt) links Coin with the Qt user interface. It allows to have OpenGL-based 3D content created with Coin displayed in a Qt widget.

Get and install the SoQt code:

```
hg clone https://bitbucket.org/Coin3D/soqt
mkdir build_soqt
cd build_soqt
setupQt5 # in some way...
cmake -DCMAKE_INSTALL_PREFIX=../install ../soqt
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
