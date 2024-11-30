################################################################################

REPO_ROOT := $(shell git rev-parse --show-toplevel)

include $(REPO_ROOT)/tools/build-tools/makefiles/help.mk

################################################################################

PROJECT_NAME = $(notdir $(CURDIR))

$(info )
$(info PROJECT=$(PROJECT_NAME))
$(info )

################################################################################

SHELL = bash

MAKECMDGOALS ?=

GNUMAKEFLAGS ?=
MAKEFLAGS ?=
MAKEFLAGS += -rR                        # do not use make's built-in rules and variables
MAKEFLAGS += -j$(shell nproc)           # parallel processing
MAKEFLAGS += -k                         # keep going on errors
MAKEFLAGS += --output-sync=recurse      # group output messages per recursive make call
MAKEFLAGS += --warn-undefined-variables

ifeq ($(MAKECMDGOALS), rebuild)
	MAKEFLAGS += -B                     # always make
endif

################################################################################

suppress_echo ?= false
verbose ?= false

ifeq ($(suppress_echo), true)
	ECHO = :
else
	ECHO = echo -e
endif

ifeq ($(verbose), true)
	Q =
else
	Q = @
	MAKEFLAGS += --no-print-directory
endif

################################################################################

SUB_TARGET_DIRS := $(realpath $(dir $(shell find -L . -mindepth 2 -maxdepth 2 -name makefile -type f)))

################################################################################

.PHONY: all help $(SUB_TARGET_DIRS) run clean rebuild

all: $(SUB_TARGET_DIRS)

help::
	$(Q) $(ECHO)
	$(Q) $(ECHO) 'OPTIONS'
	$(Q) $(ECHO) '  suppress_echo=<true>           optional, default is false'
	$(Q) $(ECHO) '  verbose=<true>                 optional, default is false'
	$(Q) $(ECHO)


$(SUB_TARGET_DIRS):
	$(Q) $(MAKE) -C $@ $(MAKECMDGOALS)

run clean rebuild: all
