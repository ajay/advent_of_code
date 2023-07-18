################################################################################

include $(dir $(realpath $(MAKEFILE_LIST)))/../build/makefile

################################################################################

INCLUDE_DIRS += -I/usr/local
CPPFLAGS += -Wno-global-constructors
LIBS += -lgtest -lgtest_main
RUN_FLAGS += --gtest_color=yes

################################################################################
