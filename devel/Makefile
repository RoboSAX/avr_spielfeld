###############################################################################
#                                                                             #
# devel/Makefile                                                              #
# ==============                                                              #
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

CURRENTDIR = devel

SUBDIRS = leander peter

###############################################################################
# define phony targets for make commands
.PHONY: all all_init  $(SUBDIRS)  clean

all: all_init $(SUBDIRS)

all_init:
	@echo "## building $(CURRENTDIR) ##"


$(SUBDIRS):
	$(MAKE) -C $@


clean:
	@echo
	@echo "## clean $(CURRENTDIR) ##"
	@for dir in $(SUBDIRS); do \
	    $(MAKE) -C $$dir clean; \
	done

###############################################################################
