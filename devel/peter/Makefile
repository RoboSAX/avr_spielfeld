###############################################################################
#                                                                             #
# devel/.../Makefile                                                          #
# ==================                                                          #
#                                                                             #
# Version: 1.0.2                                                              #
# Date   : 07.02.19                                                           #
# Author : Peter Weissig                                                      #
#                                                                             #
# Source code was taken from:                                                 #
#   https://github.com/RoboAG/avr_robolib                                     #
#                                                                             #
# See also:                                                                   #
#   https://github.com/RoboSAX/avr_spielfeld                                  #
###############################################################################

CURRENTDIR = peter

SUBDIRS = 001_master_test 002_rgb_test_simple 003_spielfeld_2019

###############################################################################
# define phony targets for make commands
.PHONY: all all_init  $(SUBDIRS)  clean

all: all_init $(SUBDIRS)

all_init:
	@echo "# building $(CURRENTDIR) #"


$(SUBDIRS):
	$(MAKE) -C $@ -f "$(ROBOLIB_MAKEFILE)"


clean:
	@echo
	@echo "# clean $(CURRENTDIR) #"
	@for dir in $(SUBDIRS); do \
	    $(MAKE) -C $$dir -f "$(ROBOLIB_MAKEFILE)" clean; \
	done

###############################################################################

