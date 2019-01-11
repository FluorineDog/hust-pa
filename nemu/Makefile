NAME = nemu
INC_DIR += ./include
BUILD_DIR ?= ./build

ifeq ($(SHARE), 1)
SO = -so
SO_CFLAGS = -fPIC -D_SHARE=1 -std=c++17
SO_LDLAGS = -shared -fPIC
endif

OBJ_DIR ?= $(BUILD_DIR)/obj$(SO)
BINARY ?= $(BUILD_DIR)/$(NAME)$(SO)

#include Makefile.git

.DEFAULT_GOAL = app

# Compilation flags
CC = g++
LD = g++
INCLUDES  = $(addprefix -I, $(INC_DIR))
CFLAGS   += -O2 -MMD -Wall -Werror -ggdb3 $(INCLUDES) -fomit-frame-pointer -Wno-restrict -std=c++17
CFLAGS   += -DDIFF_TEST_QEMU

# Files to be compiled
SRCS = $(shell find src/ -name "*.cpp")
OBJS = $(SRCS:src/%.cpp=$(OBJ_DIR)/%.o)

# Compilation patterns
$(OBJ_DIR)/%.o: src/%.cpp
	@echo + CC $<
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(SO_CFLAGS) -c -o $@ $<


# Depencies
-include $(OBJS:.o=.d)

# Some convinient rules

.PHONY: app run clean
app: $(BINARY)

override ARGS ?= -l $(BUILD_DIR)/nemu-log.txt
override ARGS += -d $(NEMU_HOME)/tools/qemu-diff/build/qemu-so

# Command to execute NEMU
NEMU_EXEC := $(BINARY) $(ARGS)

$(BINARY): $(OBJS)
	#$(call git_commit, "compile")
	@echo + LD $@
	@$(LD) -O2 -rdynamic $(SO_LDLAGS) -o $@ $^ -lSDL2 -lreadline -ldl `llvm-config --cxxflags --ldflags --system-libs --libs all`

commit: 
	#$(call git_commit, "commit")

run: $(BINARY)
	#$(call git_commit, "run")
	$(NEMU_EXEC)

gdb: $(BINARY)
	#$(call git_commit, "gdb")
	gdb -s $(BINARY) --args $(NEMU_EXEC)

clean: 
	#git clean -fd
	#git clean -fdX
	-rm -rf $(BUILD_DIR)
	$(MAKE) -C tools/gen-expr clean
	$(MAKE) -C tools/qemu-diff clean
