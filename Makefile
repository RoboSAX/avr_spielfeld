###############################################################################
#                                                                             #
# Makefile                                                                    #
# ========                                                                    #
#                                                                             #
# Version: 1.2.5                                                              #
# Date   : 07.02.18                                                           #
# Author : Peter Weissig                                                      #
#                                                                             #
# Source code was taken from:                                                 #
#   https://github.com/RoboAG/avr_robolib                                     #
#                                                                             #
# See also:                                                                   #
#   https://github.com/RoboSAX/avr_spielfeld                                  #
###############################################################################

SUBDIRS = RoboSAX devel
include Makefile.global

###############################################################################
# define phony targets for make commands
.PHONY: warn


all_init:
	$(call echoHeader,)
	$(call echoHeader, "### building $(NAME_GIT_THIS) ###")
	$(call echoHeader, "   ### sub projects: $(foreach dir,$(SUBDIRS),$(dir),) ###")

warn:
	@echo
	@echo "###### reducing output to warnings ######"
	$(MAKE) | grep -A 3 -B 3 -i warn; dummy=$?

$(SUBDIRS):
	$(MAKE) -C $@

clean:
	@echo
	@echo "### clean $(NAME_GIT_THIS) ###"
	@for dir in $(SUBDIRS); do \
	    $(MAKE) -C $$dir clean; \
	done
###############################################################################
