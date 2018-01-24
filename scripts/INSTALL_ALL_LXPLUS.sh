echo; echo

# Setting a modern 'Gcc'
echo "Setting modern 'Gcc' and 'CMake' from ATLAS..."
setupATLAS
lsetup cmake 
asetup none,gcc6
echo "'Gcc' set."

# Setup Qt5
echo "Setting 'Qt5' from CVMFS..."
export QMAKESPEC=/cvmfs/atlas-nightlies.cern.ch/repo/sw/master/sw/lcg/releases/LCG_91/qt5/5.6.2/x86_64-slc6-gcc62-opt/mkspecs/linux-g++-64
PATHQT56=/cvmfs/atlas-nightlies.cern.ch/repo/sw/master/sw/lcg/releases/LCG_91/qt5/5.6.2/x86_64-slc6-gcc62-opt
export PATH=$PATHQT56/bin:$PATH
export QTDIR=$PATHQT56
echo "'Qt5' set."

if false; then 
# Simage
echo "Downloading and compiling 'Simage'..."
hg clone https://bitbucket.org/Coin3D/simage
mkdir build_simage
cd build_simage
cmake -DCMAKE_INSTALL_PREFIX=../install ../simage
make
make install
cd ..
echo "'Simage' set."
fi


if false; then
# Coin3D
echo "Downloading and compiling 'Coin3D'..."
# Note: we cannot use "hg clone https://bitbucket.org/Coin3D/coin -r CMake" because the Mercurial version on Lxplus is way too old...
# So, we use the same method we use for compiling the Coin3D in AtlasExternals, downloading a bundled package from the VP1 web area
wget https://atlas-vp1.web.cern.ch/atlas-vp1/Coin3D_sources_AtlasExternals_build/src/coin-sources.zip
echo "Unzipping Coin3D sources..."
unzip coin-sources.zip > /dev/null 2>&1
hg clone https://bitbucket.org/Coin3D/boost-header-libs-full
hg clone https://bitbucket.org/Coin3D/generalmsvcgeneration
mkdir build_coin
cd build_coin
cmake -DCMAKE_INSTALL_PREFIX=../install ../coin
make
make install
cd ..
echo "'Coin3D' set."
fi

if false; then
# SoQt
echo "Downloading and compiling 'SoQt'..."
# as with Coin, we cannot use the simple 'hg clone https://bitbucket.org/Coin3D/soqt'
wget https://atlas-vp1.web.cern.ch/atlas-vp1/SoQt_sources_AtlasExternals_build/soqt-sources.zip
unzip soqt-sources.zip > /dev/null 2>&1
mkdir build_soqt
cd build_soqt
cmake -DCMAKE_INSTALL_PREFIX=../install ../soqt
make
make install
cd ..
echo "'SoQt' set."
fi

if true; then
# examples
echo "Compiling the examples..."
mkdir build_ex1_aa
cd build_ex1_aa
# we have to force the use of GCC6, otherwise CMake on Lxplus takes the old 4.7 in /usr/bin
cmake -DCMAKE_INSTALL_PREFIX=../install -DCMAKE_CXX_COMPILER=/cvmfs/sft.cern.ch/lcg/contrib/gcc/6.4.0/x86_64-slc6/bin/c++ ../Coin3D-SoQt-Examples/boilerplate_plus_antialiasing/
make
echo "Examples compiled."

# Run the examples
echo "Running the example..."
./customExaminerViewer
fi


