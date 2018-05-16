#!/bin/bash

### travis file for building all software modules



echo ""
echo "### Sourcing robolib"
. ../../robolib/scripts/bashrc.sh



echo ""
echo "### Building"
cd ../
PATH_SOFTWARE="$(pwd)/"
PATH_DEVEL="${PATH_SOFTWARE}devel/"



echo ""
echo "## Peter"
PATH_PETER="${PATH_DEVEL}peter/"

echo "# Teamprobe (Licht)"
cd ${PATH_PETER}licht_teamprobe/
robolib_all
if [ $? -ne 0 ]; then exit -1; fi

echo "# Test (Licht)"
cd ${PATH_PETER}licht_test/
robolib_all
if [ $? -ne 0 ]; then exit -1; fi



echo ""
echo "## Main"
PATH_MAIN="${PATH_SOFTWARE}/"

echo "# Countdown"
cd ${PATH_MAIN}Countdown/
echo "... nothing to do"
#robolib_all
if [ $? -ne 0 ]; then exit -1; fi

echo "# Robosax 2018"
cd ${PATH_MAIN}RoboSAX2018/
echo "... nothing to do"
#robolib_all
if [ $? -ne 0 ]; then exit -1; fi
