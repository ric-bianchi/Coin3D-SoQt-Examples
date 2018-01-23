# Coin3D-SoQt-Examples

## Installation

### Get the needed external dependencies

#### Install Simage

Simage is the image manipulation library within the Coin3D suite.

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

1. Get the Coin source code, from the CMake branch:

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

SoQt links Coin with the Qt user interface. It allows to have OpenGL-based 3D content created with Coin displayed in a Qt widget.

Get and install the SoQt code:

```
hg clone https://bitbucket.org/Coin3D/soqt
mkdir build_soqt
cd build_soqt
setupQt5 # in some way...
cmake -DCMAKE_INSTALL_PREFIX=../install ../soqt
```

