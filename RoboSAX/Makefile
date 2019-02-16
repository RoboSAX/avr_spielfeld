###############################################################################
#                                                                             #
# RoboSAX/Makefile                                                            #
# ================                                                            #
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

EXCLUDES = modules
INCLUDES = 

SUBDIRS = $(wildcard */)
SUBDIRS := $(subst /,,$(SUBDIRS))
SUBDIRS := $(filter-out $(EXCLUDES),$(SUBDIRS))
SUBDIRS := $(SUBDIRS) $(INCLUDES)

CURDIRSTRIP = $(subst $(MAIN_DIR),,$(CURDIR))

ifndef PARDIR
export PARDIR = 
include Makefile.global
else
include $(MAIN_DIR)/Makefile.global
endif
###############################################################################
.PHONY: $(SUBDIRS)

all_init:
	$(call echoHeader,)
	$(call echoHeader, "## folder $(NAME_GIT_THIS)$(CURDIRSTRIP) ##")
	$(call echoHeader, "   ### sub folders: $(foreach dir,$(SUBDIRS),$(dir),) ###")

$(SUBDIRS):
	$(MAKE) -C $@

clean:
	@echo
	@echo "## clean $(CURDIRSTRIP) ##"
	@for dir in $(SUBDIRS); do \
	    $(MAKE) -C $$dir clean; \
	done

###############################################################################
