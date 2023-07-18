################################################################################

include tools/makefiles/subdir/makefile

################################################################################

help::
	$(Q) $(ECHO) 'make ci                 - ci'
	$(Q) $(ECHO)
	$(Q) $(ECHO) 'make lint               - lint'
	$(Q) $(ECHO)
	$(Q) $(ECHO) 'make format             - format'
	$(Q) $(ECHO)

ci: lint
	$(Q) $(MAKE) run verbose=true

lint:
	$(Q) find . -type f \( -iname '*.h' -o -iname '*.cpp' \) -exec clang-format --style=Chromium --Werror --dry-run \{\} \+

format:
	$(Q) find . -type f \( -iname '*.h' -o -iname '*.cpp' \) -exec clang-format --style=Chromium --Werror -i \{\} \+

################################################################################
