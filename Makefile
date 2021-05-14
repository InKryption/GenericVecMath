# A fairly simple, generic makefile designed with the intent of compiling C++ programs, but which I believe should serve me through other purposes as well.

.ONESHELL: # Make everything happen in the same shell

# DEFINITION REQUISITES:

# 'COMPILER': Compiler path
# 'ROOT': Root directory of the project
# 'BIN': Bin folder relative to $(ROOT)/
# 'SRC': Source files folder relative to $(ROOT)/
# 'BUILD': Name of the build
# 'PROJECT_NAME': Name of the project / resulting executable

# OTHER DEFINITIONS

# 'LFLAG': linker flags
# 'CFLAG': compiler flags
# 'EXECUTABLE_SHELL': Shell in which to execute the program
# 'PROJECT_NAME_PREFIX': Prefix to append to the name of the executable
# 'PROJECT_NAME_POSTFIX': Postfix to append to the name of the executable

include .make/Config.mk
$(eval include .make/$(if $(config),$(config).mk,$(DEFAULT_CONFIG).mk))

# Internal definitions

define All_Files_Inside
$(foreach file,$(wildcard $(1)/*),$(if $(wildcard $(file)/*),$(call All_Files_Inside,$(file)),$(file)))
endef

define Compile_Source
$(OBJDIR)/$(notdir $(firstword $(subst ., ,$(1)))).o: $(1) $(filter %$(notdir $(firstword $(subst ., ,$(1)))).hpp,$(call All_Files_Inside,$(SRC_FOLDER)))
	$(COMPILER) -c $(1) -o $(OBJDIR)/$(notdir $(firstword $(subst ., ,$(1)))).o $(COMPILER_FLAGS)
endef

SRC_FOLDER:=$(ROOT)/$(SRC)
BIN_FOLDER:=$(ROOT)/$(BIN)

ODIR:=$(BIN_FOLDER)/$(BUILD)
OBJDIR:=$(ODIR)/obj
EXECUTABLE:=$(ODIR)/$(PROJECT_NAME_PREFIX)$(PROJECT_NAME)$(PROJECT_NAME_POSTFIX)

SOURCE_FILES:=$(filter %.cpp,$(call All_Files_Inside,$(SRC_FOLDER)))
OBJECT_FILES:=$(foreach src,$(SOURCE_FILES),$(OBJDIR)/$(firstword $(subst ., ,$(notdir $(src)))).o)
DIRTY_OBJECTS=$(foreach pdo,$(wildcard $(OBJDIR)/*),$(if $(filter $(pdo),$(OBJECT_FILES)),,$(pdo))) $(filter-out $(EXECUTABLE),$(wildcard $(ODIR)/*.exe))

LINKER_FLAGS:=$(LFLAG)
COMPILER_FLAGS:=$(CFLAG)

# Main Targets

$(EXECUTABLE): $(OBJECT_FILES)
	-$(COMPILER) $(OBJECT_FILES) -o $(EXECUTABLE) $(LINKER_FLAGS)

$(foreach src,$(SOURCE_FILES),$(eval $(call Compile_Source,$(src))))



# Utility Targets:
.PHONY: clean dir run

# Run the executable via powershell
run: $(EXECUTABLE)
	$(EXECUTABLE_SHELL) $(EXECUTABLE) $(ARG)

# Create directory for the output if it doesn't exist
dir:
	-mkdir -p $(OBJDIR)

# Clean output
clean:
	$(foreach objfile,$(DIRTY_OBJECTS),@if [ -f "$(objfile)" ]; then rm $(objfile); echo "Removed $(objfile)"; fi)
