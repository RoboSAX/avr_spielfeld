###############################################################################
#                                                                             #
# Makefile                                                                    #
# ========                                                                    #
#                                                                             #
# Version: 1.3.0                                                              #
# Date   : 22.02.25                                                           #
# Author : Peter Weissig, Leander Herr                                        #
#                                                                             #
# Source code was taken from:                                                 #
#   https://github.com/RoboAG/avr_robolib                                     #
#                                                                             #
# See also:                                                                   #
#   https://github.com/RoboSAX/avr_spielfeld                                  #
###############################################################################

SUBDIRS = RoboSAX
include Makefile.global



###############################################################################
.PHONY: all_init clean clang-format-all

all_init:
	$(call echoHeader,)
	$(call echoHeader, "### building $(NAME_GIT_THIS) ###")
	$(call echoHeader, "   ### sub projects: $(foreach dir,$(SUBDIRS),$(dir),) ###")

$(SUBDIRS):
	$(MAKE) -C $@

clean:
	@echo
	@echo "### clean $(NAME_GIT_THIS) ###"
	@for dir in $(SUBDIRS); do \
	    $(MAKE) -C $$dir clean; \
	done

clang-format-all:
	@echo
	@echo "### clang-format ###"
	@find RoboSAX/ -type f -iname "*.[ch]" -exec clang-format-18 --verbose -i {} \;



###############################################################################
