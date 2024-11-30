# set directories
WORK_DIR = $(shell pwd)
BUILD_DIR = $(WORK_DIR)/build
RESOURCE_DIR = $(WORK_DIR)/resources

# create build directory
$(shell mkdir -p $(BUILD_DIR))

STAGE = game
MODULES = common minitui $(STAGE)

# include paths
INC_PATH = $(addsuffix /include, $(MODULES))
INC_FLAG = $(addprefix -I, $(INC_PATH))

# flags

# block some warnings if needed
ifdef FEW_WARNINGS
  NO_WARN_FLAGS = -Wno-unused-variable -Wno-unused-result -Wno-unused-but-set-variable -Wno-unused-parameter
else
  NO_WARN_FLAGS =
endif

ifdef TEST
  DEFINES += TEST_MODE
endif

COMMON_FLAGS = -O2 -ggdb -MMD -Wall -Wextra $(NO_WARN_FLAGS) $(INC_FLAG) $(addprefix -D, $(DEFINES))

CFLAGS = $(COMMON_FLAGS)
CXXFLAGS = $(COMMON_FLAGS) -std=c++20

# compile tools
CC = gcc
CXX = g++
AS = as
LD = ld

# common objects

C_SRCS = $(foreach module, $(MODULES), $(shell find $(module)/source -name *.c))
CXX_SRCS = $(foreach module, $(MODULES), $(shell find $(module)/source -name *.cpp))
OBJS = $(addprefix $(BUILD_DIR)/, $(C_SRCS:%.c=%.o) $(CXX_SRCS:%.cpp=%.o))

ifeq ($(shell echo $$OS), Windows_NT)
# resource objects (only for windows)
RESS = $(shell find resources -name *.rc) 
OBJS += $(addprefix $(BUILD_DIR)/, $(RESS:%.rc=%.o))
else
endif

include scripts/gsetting.mk

# rules

$(BUILD_DIR)/%.o : %.cpp
	@mkdir -p $(dir $@) && echo + $(CXX) $<
	@$(CXX) -std=c++14 -c -o $@ $(CXXFLAGS) $<
# convert windows paths to unix paths in dependency files
ifeq ($(shell echo $$OS), Windows_NT)
	@sed -i 's/[A-Z]:\//\/\l&/g' $(patsubst %.o, %.d, $@)
	@sed -i 's/:\//\//g' $(patsubst %.o, %.d, $@)
endif

$(BUILD_DIR)/%.o : %.c
	@mkdir -p $(dir $@) && echo + $(CC) $<
	@$(CC) -std=gnu11 -c -o $@ $(CFLAGS) $<
# convert windows paths to unix paths in dependency files
ifeq ($(shell echo $$OS), Windows_NT)
	@sed -i 's/[A-Z]:\//\/\l&/g' $(patsubst %.o, %.d, $@)
	@sed -i 's/:\//\//g' $(patsubst %.o, %.d, $@)
endif
	
ifeq ($(shell echo $$OS), Windows_NT)

$(BUILD_DIR)/resources/%.o : resources/%.rc
	@mkdir -p $(dir $@) && echo + windres $<
	@windres -i $(realpath $<) -o $@

endif

# targets

# $(BUILD_DIR)/wrapper/shell.o

$(BUILD_DIR)/shell: $(BUILD_DIR)/wrapper/shell.o
	@$(CXX) -o $@ $<

$(BUILD_DIR)/nju_universalis: $(OBJS)
	@$(CXX) -o $@ $(OBJS)

shell: $(BUILD_DIR)/shell compile
	@$(BUILD_DIR)/shell

test_shell:
	@$(MAKE) shell TEST=1

test_compile:
	@$(MAKE) compile TEST=1

compile: $(BUILD_DIR)/nju_universalis
	@echo Compilation finished!

# run: compile
# 	@$(BUILD_DIR)/nju_universalis 2>$(BUILD_DIR)/game.log

-include $(addprefix $(BUILD_DIR)/, $(addsuffix .d, $(basename $(SRCS))))

clean:
	-@rm -rf build

.PHONY: clean compile run test shell
