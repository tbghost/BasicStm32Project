#!/bin/bash

set -e


this_path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )
. $this_path/colorcodes.sh
. $this_path/cmake_init_unittest.sh
prj_root_path="${this_path%/*}"

echo " "
echo "--------------------------------"
echo "---    Build Unittests      ----"
echo "--- PRJ_ROOT_PATH: " $prj_root_path
cd $prj_root_path/build

cmake --build . --parallel 4

unittests=$(find "$prj_root_path/build/tests" -type f -name "gTestUnit")
if [ -f "$unittests" ]; 
then
  echo -e "${Green}--- ${unittests} ---${Color_Off}"
  echo -e "${Green}--- Build succeeded ---${Color_Off}"
  echo " "
else
  echo -e "${Red}--- Build failed ---${Color_Off}"
fi

