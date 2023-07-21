################################################################################

# Prints the version of a tool to stdout. Assumes support for the --version flag.
# $(1) = string of tool variable name (e.g. CC, CXX, etc.)
# $(2) = string of tool (e.g. clang, gcc, etc.)
define print_tool_version
	@echo -e "$(1)=$(2)"
	@echo -e "`command -v $(2)`"
	@echo -e "`$(2) --version`\n"
endef

################################################################################
