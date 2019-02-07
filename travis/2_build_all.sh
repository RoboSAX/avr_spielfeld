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
PATH_MAIN="${PATH_SOFTWARE}RoboSAX/"



echo ""
echo "## Peter"
PATH_PETER="${PATH_DEVEL}peter/"

echo "# Test master"
cd ${PATH_PETER}001_master_test/
if [ $? -ne 0 ]; then exit -1; fi
robolib_all
if [ $? -ne 0 ]; then exit -1; fi

echo "# Test ledbox simple"
cd ${PATH_PETER}002_rgb_test_simple/
if [ $? -ne 0 ]; then exit -1; fi
robolib_all
if [ $? -ne 0 ]; then exit -1; fi

echo "# Devel Spielfeld 2019"
cd ${PATH_PETER}003_spielfeld_2019/
if [ $? -ne 0 ]; then exit -1; fi
robolib_all
if [ $? -ne 0 ]; then exit -1; fi



echo ""
echo "## Daniel"
PATH_DANIEL="${PATH_DEVEL}daniel/"



echo ""
echo "## Leander"
PATH_LEANDER="${PATH_DEVEL}leander/"

echo "# Test Licht"
cd ${PATH_LEANDER}001_licht_test/
if [ $? -ne 0 ]; then exit -1; fi
robolib_all
if [ $? -ne 0 ]; then exit -1; fi

# unfixed build error
#echo "# Test Spielfeld"
#cd ${PATH_LEANDER}002_spielfeld/
#if [ $? -ne 0 ]; then exit -1; fi
#robolib_all
#if [ $? -ne 0 ]; then exit -1; fi

echo "# Devel Spielfeld 2019"
cd ${PATH_LEANDER}003_2019/Spielfeld/
if [ $? -ne 0 ]; then exit -1; fi
robolib_all
if [ $? -ne 0 ]; then exit -1; fi



echo ""
echo "## RoboSAX 2018"
PATH_ROBOSAX2018="${PATH_MAIN}2018/"

echo "# Spielfeld 2018"
cd ${PATH_ROBOSAX2018}Spielfeld/src/
if [ $? -ne 0 ]; then exit -1; fi
robolib_all
if [ $? -ne 0 ]; then exit -1; fi

echo "# Teamprobe 2018 - Main"
cd ${PATH_ROBOSAX2018}Teamprobe/Main/src/
if [ $? -ne 0 ]; then exit -1; fi
robolib_all
if [ $? -ne 0 ]; then exit -1; fi

echo "# Teamprobe 2018 - Test"
cd ${PATH_ROBOSAX2018}Teamprobe/Test/src/
if [ $? -ne 0 ]; then exit -1; fi
robolib_all
if [ $? -ne 0 ]; then exit -1; fi

echo ""
echo "## RoboSAX 2019"
PATH_ROBOSAX2018="${PATH_MAIN}2019/"

echo "# Spielfeld 2019"
cd ${PATH_ROBOSAX2018}Spielfeld/src/
if [ $? -ne 0 ]; then exit -1; fi
robolib_all
if [ $? -ne 0 ]; then exit -1; fi


