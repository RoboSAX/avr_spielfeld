#!/bin/bash

###############################################################################
#                                                                             #
# .robolib.sh                                                                 #
# ===========                                                                 #
#                                                                             #
# Version: 1.2.0                                                              #
# Date   : 08.02.18                                                           #
# Author : Peter Weissig                                                      #
#                                                                             #
# See also:                                                                   #
#   https://github.com/RoboSAX/avr_spielfeld                                  #
###############################################################################



### download
echo ""
echo "### Downloading robolib"
echo ""
if [ -d robolib ]; then
    echo "robolib already downloaded"
else
    git clone https://github.com/RoboAG/avr_robolib robolib
    if [ $? -ne 0 ]; then return -1; exit -1; fi
fi



### install
echo ""
echo "### Configuring robolib"
echo ""
cd robolib
make install_prerequisites
if [ $? -ne 0 ]; then return -1; exit -1; fi



### source
echo ""
echo "### Sourcing robolib"
echo ""
. scripts/bashrc.sh
if [ $? -ne 0 ]; then return -1; exit -1; fi

