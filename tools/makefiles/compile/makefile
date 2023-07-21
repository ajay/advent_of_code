################################################################################

PROJECT_NAME = $(notdir $(CURDIR))

$(info )
$(info PROJECT=$(PROJECT_NAME))
$(info )

################################################################################

SHELL = bash

MAKECMDGOALS ?=

MAKEFLAGS += -rR                        # do not use make's built-in rules and variables
MAKEFLAGS += -j                         # parallel processing
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

CC  = clang
CXX = clang++

CPPFLAGS  = -MMD
CPPFLAGS += -Wall
CPPFLAGS += -Werror
CPPFLAGS += -Weverything
CPPFLAGS += -Wno-missing-prototypes
CPPFLAGS += -fPIC
CPPFLAGS += $(INCLUDE_DIRS)

CFLAGS  = -std=c17
CFLAGS += -fgnu89-inline

CXXFLAGS  = -std=gnu++2b
CXXFLAGS += -Wno-c++98-compat
CXXFLAGS += -Wno-c++98-compat-pedantic

LDFLAGS = -pie

LIBS = -lfmt

RUN_FLAGS =

################################################################################

BUILD_DIR = build
SRC_DIR = src
OBJ_DIR = $(BUILD_DIR)/obj
OUT_DIR = $(BUILD_DIR)/out

################################################################################

SRC_FILE_EXTS = .asm .c .cpp

INCLUDE_DIRS := -I$(SRC_DIR)

SRCS := $(foreach ext, $(SRC_FILE_EXTS),    \
        $(shell test -d $(SRC_DIR) && find -L $(SRC_DIR) -type f -name '*$(ext)' -printf '%p\n'))

OBJS := $(sort                              \
        $(addprefix $(OBJ_DIR)/,            \
        $(addsuffix .o,                     \
        $(basename                          \
        $(SRCS:$(SRC_DIR)/%=%)))))

TARGET = $(OUT_DIR)/$(PROJECT_NAME)

################################################################################

include $(dir $(realpath $(word 1,$(MAKEFILE_LIST))))/../functions/makefile

################################################################################

.PHONY: all help run clean rebuild

all: $(TARGET)

help:
	$(Q) $(ECHO)
	$(Q) $(ECHO) 'make                    - make all'
	$(Q) $(ECHO) '  suppress_echo=<true>  - optional, default is false'
	$(Q) $(ECHO) '  verbose=<true>        - optional, default is false'
	$(Q) $(ECHO)
	$(Q) $(ECHO) 'make run                - run'
	$(Q) $(ECHO)
	$(Q) $(ECHO) 'make clean              - clean'
	$(Q) $(ECHO)
	$(Q) $(ECHO) 'make rebuild            - rebuild'
	$(Q) $(ECHO)
	$(Q) $(ECHO) 'make versions           - prints tool versions'
	$(Q) $(ECHO)
	$(Q) $(ECHO) 'make help               - this info'
	$(Q) $(ECHO)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.asm
	$(Q) $(ECHO) '(ASM)' $<
	$(Q) mkdir -p $(@D)
	$(Q) $(CC) -c $(CPPFLAGS) $(CFLAGS) -o $@ $<

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(Q) $(ECHO) '(CC) ' $<
	$(Q) mkdir -p $(@D)
	$(Q) $(CC) -c $(CPPFLAGS) $(CFLAGS) -o $@ $<

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(Q) $(ECHO) '(CXX)' $<
	$(Q) mkdir -p $(@D)
	$(Q) $(CXX) -c $(CPPFLAGS) $(CXXFLAGS) -o $@ $<

$(TARGET): $(OBJS)
	$(Q) $(ECHO) '(LINK)' $@
	$(Q) mkdir -p $(@D)
	$(Q) $(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

run: $(TARGET)
	$(Q) $(ECHO) '(RUN)' $<
	$(Q) $(TARGET) $(RUN_FLAGS)

clean:
	$(Q) rm -rf $(BUILD_DIR)

rebuild: clean all

versions:
	$(call print_tool_version,CC,$(CC))
	$(call print_tool_version,CXX,$(CXX))

-include $(OBJS:.o=.d) # compiler generated dependency info
