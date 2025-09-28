#!/bin/bash

set -e

this_path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )
. $this_path/colorcodes.sh
prj_root_path="${this_path%/*}"
cd $prj_root_path

echo " "
echo "--------------------------------"
echo "--- Exec UnitTests     ---------"

unittests=$(find "$prj_root_path/build/tests" -type f -name "gTestUnit")
if [ -f "$unittests" ]; 
then
  echo -e "${Green}--- ${unittests} ---${Color_Off}"
  cd ./build/tests
  echo "--------------------------------"
  echo " "
  ./gTestUnit $@
else
  echo -e "${Red}--- Unittests not found ---${Color_Off}"
fi





