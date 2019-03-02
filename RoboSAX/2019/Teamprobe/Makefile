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

EXCLUDES = 
INCLUDES = 

SUBDIRS = $(wildcard */)
SUBDIRS := $(subst /,,$(SUBDIRS))
SUBDIRS := $(filter-out $(EXCLUDES),$(SUBDIRS))
SUBDIRS := $(SUBDIRS) $(INCLUDES)

DOWNLOAD_PREFIX = Download_
AVR_SUBDIRS = $(patsubst %,$(DOWNLOAD_PREFIX)%,$(SUBDIRS))

CURDIRSTRIP = $(subst $(MAIN_DIR),,$(CURDIR))

ifndef PARDIR
export PARDIR = 
include Makefile.global
else
include $(MAIN_DIR)/Makefile.global
endif
###############################################################################
.PHONY: $(SUBDIRS) $(AVR_SUBDIRS)
all_init:
	$(call echoHeader,)
	$(call echoHeader, "# folder $(NAME_GIT_THIS)$(CURDIRSTRIP) #")

$(SUBDIRS):
	$(call echoHeader, "compile code for $(NAME_GIT_THIS)/$(CURDIRSTRIP)/$(SUBDIR)")
	$(MAKE) -C $@ -f "$(ROBOLIB_MAKEFILE)"

$(AVR_SUBDIRS):	
	$(eval DIR=$(subst $(DOWNLOAD_PREFIX),,$@))
	$(MAKE) $(DIR)
	$(call echoHeader, "download code for $(NAME_GIT_THIS)/$(CURDIRSTRIP)/$(DIR)")
	$(MAKE) avrdude -C $(DIR) -f "$(ROBOLIB_MAKEFILE)"

clean:
	@echo
	@echo "## clean $(CURDIRSTRIP) ##"
	@for dir in $(SUBDIRS); do \
	    $(MAKE) -C $$dir -f "$(ROBOLIB_MAKEFILE)" clean; \
	done

###############################################################################
