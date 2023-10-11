WORK_DIR = $(shell pwd)
BUILD_DIR = $(WORK_DIR)/build
RESOURCE_DIR = $(WORK_DIR)/resources

$(shell mkdir -p $(BUILD_DIR))

STAGE = game

INC_PATH = $(realpath minitui/include) $(realpath $(STAGE)/include)
INC_FLAG = $(addprefix -I, $(INC_PATH))

COMMON_FLAGS = -O2 -ggdb -MMD -Wall -Wno-unused-variable -Wno-unused-result -Wno-unused-but-set-variable -Wno-unused-parameter -Wextra $(INC_FLAG)
CFLAGS = $(COMMON_FLAGS)
CXXFLAGS = $(COMMON_FLAGS)

CC = gcc
CXX = g++
AS = as
LD = ld

SRCS = $(shell find minitui/source -name *.cpp) $(shell find minitui/source -name *.c) $(shell find $(STAGE)/source -name *.c) $(shell find $(STAGE)/source -name *.cpp)
OBJS = $(addprefix $(BUILD_DIR)/, $(addsuffix .o, $(basename $(SRCS))))


ifeq ($(shell echo $$OS), Windows_NT)

RESS = $(shell find resources -name *.rc) 
OBJS += $(addprefix $(BUILD_DIR)/, $(patsubst %.rc, %.o, $(RESS)))

endif

compile: $(BUILD_DIR)/nju_universalis
	@echo Compilation finished!

run: compile
	@$(BUILD_DIR)/nju_universalis 2>$(BUILD_DIR)/game.log

$(BUILD_DIR)/shell:  wrapper/shell.c
	@gcc -Iminitui/include -o $(BUILD_DIR)/shell wrapper/shell.c

shell: $(BUILD_DIR)/shell
	@$(BUILD_DIR)/shell

$(BUILD_DIR)/nju_universalis: $(OBJS)
	@$(CXX) -o $@ $(OBJS)

$(BUILD_DIR)/%.o : %.cpp
	@mkdir -p $(dir $@) && echo + $(CXX) $<
	@$(CXX) -std=c++14 -c -o $@ $(CXXFLAGS) $(realpath $<)
	@sed -i 's/[A-Z]:\//\/\l&/g' $(patsubst %.o, %.d, $@)
	@sed -i 's/:\//\//g' $(patsubst %.o, %.d, $@)

$(BUILD_DIR)/%.o : %.c
	@mkdir -p $(dir $@) && echo + $(CC) $<
	@$(CC) -std=gnu11 -c -o $@ $(CFLAGS) $(realpath $<)
	@sed -i 's/[A-Z]:\//\/\l&/g' $(patsubst %.o, %.d, $@)
	@sed -i 's/:\//\//g' $(patsubst %.o, %.d, $@)
	
ifeq ($(shell echo $$OS), Windows_NT)

$(BUILD_DIR)/resources/%.o : resources/%.rc
	@mkdir -p $(dir $@) && echo + windres $<
	@windres -i $(realpath $<) -o $@

endif

-include $(addprefix $(BUILD_DIR)/, $(addsuffix .d, $(basename $(SRCS))))

clean:
	-@rm -rf build

.PHONY: clean compile run
