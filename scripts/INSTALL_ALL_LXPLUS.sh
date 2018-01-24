
# Setup Qt5
setupATLAS
export QMAKESPEC=/cvmfs/atlas-nightlies.cern.ch/repo/sw/master/sw/lcg/releases/LCG_91/qt5/5.6.2/x86_64-slc6-gcc62-opt/mkspecs/linux-g++-64
PATHQT56=/cvmfs/atlas-nightlies.cern.ch/repo/sw/master/sw/lcg/releases/LCG_91/qt5/5.6.2/x86_64-slc6-gcc62-opt
export PATH=$PATHQT56/bin:$PATH
export QTDIR=$PATHQT56

# Simage
hg clone https://bitbucket.org/Coin3D/simage
mkdir build_simage
cd build_simage
cmake -DCMAKE_INSTALL_PREFIX=../install ../simage
make
make install
cd ..

# Coin3D
hg clone https://bitbucket.org/Coin3D/coin -r CMake
hg clone https://bitbucket.org/Coin3D/boost-header-libs-full
hg clone https://bitbucket.org/Coin3D/generalmsvcgeneration
mkdir build_coin
cd build_coin
cmake -DCMAKE_INSTALL_PREFIX=../install ../coin
make
make install
cd ..

# SoQt
hg clone https://bitbucket.org/Coin3D/soqt
mkdir build_soqt
cd build_soqt
cmake -DCMAKE_INSTALL_PREFIX=../install ../soqt
make
make install

# examples
mkdir build_ex1_aa
cd build_ex1_aa
cmake -DCMAKE_INSTALL_PREFIX=../install ../Coin3D-SoQt-Examples/boilerplate_plus_antialiasing/
make

# Run the examples
./customExaminerViewer
