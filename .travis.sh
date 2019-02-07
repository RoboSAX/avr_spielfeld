#!/bin/bash



### config
PWD_temp="$(pwd)"
mkdir -p travis/ && cd travis/



### robolib
echo ""
echo "### Downloading robolib"
echo ""
if [ -d robolib ]; then
    echo "robolib already downloaded"
else
    git clone https://github.com/RoboAG/avr_robolib robolib
    if [ $? -ne 0 ]; then cd "${PWD_temp}"; return -1; exit -1; fi
fi

echo ""
echo "### Configuring robolib"
echo ""
cd robolib
make install_prerequisites
if [ $? -ne 0 ]; then cd "${PWD_temp}"; return -1; exit -1; fi


echo ""
echo "### Sourcing robolib"
echo ""
. scripts/bashrc.sh
if [ $? -ne 0 ]; then cd "${PWD_temp}"; return -1; exit -1; fi



### build
echo ""
echo "### Building"
echo ""
cd "${PWD_temp}"
make



### done
if [ $? -ne 0 ]; then
    echo ""
    echo "### errors occurred  :-("
    echo ""
    exit -1
else
    echo ""
    echo "### all done :-)"
    echo ""
fi
