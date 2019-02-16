###############################################################################
#                                                                             #
# RoboSAX/.../Makefile                                                        #
# ====================                                                        #
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

SUBDIRS = Spielfeld/src Teamprobe/Main/src Teamprobe/Test/src

###############################################################################
# define phony targets for make commands
.PHONY: all all_init  $(SUBDIRS)  clean

all: all_init $(SUBDIRS)

all_init:
	@echo "## building $(CURDIRSTRIP) ##"


$(SUBDIRS):
	$(MAKE) -C $@ -f "$(ROBOLIB_MAKEFILE)"


clean:
	@echo
	@echo "## clean $(CURDIRSTRIP) ##"
	@for dir in $(SUBDIRS); do \
	    $(MAKE) -C $$dir -f "$(ROBOLIB_MAKEFILE)" clean; \
	done

###############################################################################
include Makefile.global
