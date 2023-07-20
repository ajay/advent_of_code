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

SOURCE_DIR = src
BUILD_DIR = build
OBJECT_DIR = $(BUILD_DIR)/obj
OUT_DIR = $(BUILD_DIR)/out

################################################################################

SRC_FILE_EXTS = .asm .c .cpp

INCLUDE_DIRS := -I$(SOURCE_DIR)

OBJS         := $(sort                              \
                $(addprefix $(OBJECT_DIR)/,         \
                $(addsuffix .o,                     \
                $(basename                          \
                $(foreach ext, $(SRC_FILE_EXTS),    \
                $(shell test -d $(SOURCE_DIR) && find -L $(SOURCE_DIR) -type f -name '*$(ext)' -printf '%P\n'))))))

TARGET = $(OUT_DIR)/$(PROJECT_NAME)

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
	$(Q) $(ECHO) 'make help               - this info'
	$(Q) $(ECHO)

$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.asm
	$(Q) $(ECHO) '(ASM)' $<
	$(Q) mkdir -p $(@D)
	$(Q) $(CC) -c $(CPPFLAGS) $(CFLAGS) -o $@ $<

$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.c
	$(Q) $(ECHO) '(CC) ' $<
	$(Q) mkdir -p $(@D)
	$(Q) $(CC) -c $(CPPFLAGS) $(CFLAGS) -o $@ $<

$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.cpp
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