#!/bin/bash
set -e

this_path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )
. $this_path/colorcodes.sh
. $this_path/clean_all.sh
prj_root_path="${this_path%/*}"

echo " "
echo "--------------------------------"
echo "--- Config CMAKE Unittests  ----"
echo "--- PRJ_ROOT_PATH: " $prj_root_path
cd $prj_root_path

# generate a cmake configuration to build unittest debug
mkdir build
cd build
echo "--------------------------------"

cmake -DCMAKE_BUILD_TYPE="Debug" -DPLATFORM="Unittest" -DCMAKE_TOOLCHAIN_FILE="${prj_root_path}/cmake/gcc-native-linux.cmake" ..

