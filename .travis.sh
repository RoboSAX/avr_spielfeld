#!/bin/bash

###############################################################################
#                                                                             #
# .travis.sh                                                                  #
# ==========                                                                  #
#                                                                             #
# Version: 1.2.2                                                              #
# Date   : 09.02.18                                                           #
# Author : Peter Weissig                                                      #
#                                                                             #
# See also:                                                                   #
#   https://github.com/RoboSAX/avr_spielfeld                                  #
###############################################################################



### config
PWD_temp="$(pwd)"



### robolib
echo ""
echo "###### robolib"
echo ""
if [ "${ROBOLIB_MAKEFILE}" != "" ]; then
    echo "robolib already sourced :-)"
else
    . .robolib.sh
    if [ $? -ne 0 ]; then cd "${PWD_temp}"; return -1; exit -1; fi
fi



### build
echo ""
echo "###### Building"
echo ""
cd "${PWD_temp}"
make



### done
if [ $? -ne 0 ]; then
    echo ""
    echo "###### errors occurred  :-("
    echo ""
    return -1
    exit   -1
else
    echo ""
    echo "###### all done :-)"
    echo ""
fi
