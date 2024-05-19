################################################################################

include tools/makefiles/functions/makefile
include tools/makefiles/subdir/makefile

################################################################################

CXX_FORMATTER = $(shell command -v clang-format-17 &> /dev/null && echo "clang-format-17" || echo "clang-format")

CXX_FORMATTER_FLAGS += --style='{BasedOnStyle: Chromium, ColumnLimit: 100}'
CXX_FORMATTER_FLAGS += --Werror
CXX_FORMATTER_FLAGS += -i

PYTHON_FORMATTER = python3 -m black

PYTHON_FORMATTER_FLAGS +=

ifneq ($(findstring lint, $(MAKECMDGOALS)),)
	CXX_FORMATTER_FLAGS += --dry-run
	PYTHON_FORMATTER_FLAGS += --check
endif

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
	$(Q) $(ECHO) 'make setup-day          - sets up a skeleton for an aoc day'
	$(Q) $(ECHO) '  year=<XXXX>           - required, integer'
	$(Q) $(ECHO) '  day=<XX>              - required, integer'
	$(Q) $(ECHO)

################################################################################

year ?=
day ?=

ifeq ($(MAKECMDGOALS),setup-day)

ifeq ($(year),)
$(error $(MAKECMDGOALS): year is required)
endif

ifeq ($(day),)
$(error $(MAKECMDGOALS): day is required)
endif

endif

setup-day:
	$(Q) tools/scripts/aoc_setup_day.py --year $(year) --day $(day) --repo-root-path $(shell git rev-parse --show-toplevel)

################################################################################

install-deps:
	$(Q) tools/deps/os/$(OS).sh
	$(Q) python3 -m pip install -r tools/deps/python/requirements.txt

ci:
	$(Q) $(MAKE) versions verbose=true
	$(Q) $(MAKE) lint verbose=true
	$(Q) $(MAKE) run verbose=true

lint: _lint_cxx _lint_python
format: _format_cxx _format_python
_lint_cxx _format_cxx:
	$(Q) find . -type f \( -iname '*.h' -o -iname '*.cpp' \) -exec $(CXX_FORMATTER) $(CXX_FORMATTER_FLAGS) \{\} \+
_lint_python _format_python:
	$(Q) find . -type f -name '*.py' -exec $(PYTHON_FORMATTER) $(PYTHON_FORMATTER_FLAGS) \{\} \+

versions:
	$(call print_tool_version,CXX_FORMATTER,$(CXX_FORMATTER))
	$(call print_tool_version,PYTHON_FORMATTER,$(PYTHON_FORMATTER))
	$(Q) $(MAKE) -C tools/makefiles/compile versions verbose=true

################################################################################
