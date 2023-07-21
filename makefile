################################################################################

include tools/makefiles/subdir/makefile

################################################################################

CXX_FORMATTER = $(shell command -v clang-format-15 &> /dev/null && echo "clang-format-15" || echo "clang-format")

CXX_FORMATTER_FLAGS  = --style=Chromium
CXX_FORMATTER_FLAGS += --Werror
CXX_FORMATTER_FLAGS += -i

################################################################################

help::
	$(Q) $(ECHO) 'make ci                 - ci'
	$(Q) $(ECHO)
	$(Q) $(ECHO) 'make lint               - lint'
	$(Q) $(ECHO)
	$(Q) $(ECHO) 'make format             - format'
	$(Q) $(ECHO)

ci:
	$(Q) $(MAKE) lint verbose=true
	$(Q) $(MAKE) run verbose=true

lint:
	$(Q) find . -type f \( -iname '*.h' -o -iname '*.cpp' \) -exec $(CXX_FORMATTER) $(CXX_FORMATTER_FLAGS) --dry-run \{\} \+

format:
	$(Q) find . -type f \( -iname '*.h' -o -iname '*.cpp' \) -exec $(CXX_FORMATTER) $(CXX_FORMATTER_FLAGS) \{\} \+

################################################################################
