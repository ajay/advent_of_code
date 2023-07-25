################################################################################

include tools/makefiles/functions/makefile
include tools/makefiles/subdir/makefile

################################################################################

CXX_FORMATTER = $(shell command -v clang-format-15 &> /dev/null && echo "clang-format-15" || echo "clang-format")

CXX_FORMATTER_FLAGS  = --style=Chromium
CXX_FORMATTER_FLAGS += --Werror
CXX_FORMATTER_FLAGS += -i

################################################################################

OS := $(shell lsb_release --id | awk '{print $$3}' | tr A-Z a-z)

################################################################################

help::
	$(Q) $(ECHO) 'make install-deps       - install deps'
	$(Q) $(ECHO)
	$(Q) $(ECHO) 'make ci                 - ci'
	$(Q) $(ECHO)
	$(Q) $(ECHO) 'make lint               - lint'
	$(Q) $(ECHO)
	$(Q) $(ECHO) 'make format             - format'
	$(Q) $(ECHO)
	$(Q) $(ECHO) 'make versions           - prints tool versions'
	$(Q) $(ECHO)

install-deps:
	$(Q) tools/deps/os/$(OS).sh
	$(Q) python3 -m pip install -r tools/deps/python/requirements.txt

ci:
	$(Q) $(MAKE) versions verbose=true
	$(Q) $(MAKE) lint verbose=true
	$(Q) $(MAKE) run verbose=true

lint:
	$(Q) find . -type f \( -iname '*.h' -o -iname '*.cpp' \) -exec $(CXX_FORMATTER) $(CXX_FORMATTER_FLAGS) --dry-run \{\} \+

format:
	$(Q) find . -type f \( -iname '*.h' -o -iname '*.cpp' \) -exec $(CXX_FORMATTER) $(CXX_FORMATTER_FLAGS) \{\} \+

versions:
	$(call print_tool_version,CXX_FORMATTER,$(CXX_FORMATTER))
	$(Q) $(MAKE) -C tools/makefiles/compile versions verbose=true

################################################################################
