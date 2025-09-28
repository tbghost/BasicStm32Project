#!/bin/bash

set -e

this_path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )
. $this_path/colorcodes.sh

prj_root_path="${this_path%/*}"
cd $prj_root_path
build_dirs=$(find "$prj_root_path" -type d -name "build*")

echo " "
echo "--------------------------------"
echo "--- Remove build directories ---"
echo "--- PRJ_ROOT_PATH: " $prj_root_path

if [ -z "$build_dirs" ]; 
then
  echo -e "${Red}--- Build directories not found ---${Color_Off}"
else
  echo -e "${Green}--- Remove Build directories ---${Color_Off}"
  echo "$build_dirs"
  rm -fr build*  
fi
echo "--------------------------------"