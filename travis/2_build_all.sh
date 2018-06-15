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

echo "# Test master"
cd ${PATH_PETER}master_test/
robolib_all
if [ $? -ne 0 ]; then exit -1; fi

echo "# Test ledbox simple"
cd ${PATH_PETER}rgb_test_simple/
robolib_all
if [ $? -ne 0 ]; then exit -1; fi



echo ""
echo "## Leander"
PATH_LEANDER="${PATH_DEVEL}leander/"

echo "# Final Version"
cd ${PATH_LEANDER}THE_GAME/
robolib_all
if [ $? -ne 0 ]; then exit -1; fi



echo ""
echo "## Main"
PATH_MAIN="${PATH_SOFTWARE}/"

echo "# Robosax 2018"
cd ${PATH_MAIN}RoboSAX2018/
echo "... nothing to do"
#robolib_all
if [ $? -ne 0 ]; then exit -1; fi
