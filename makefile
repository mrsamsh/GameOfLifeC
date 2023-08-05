CFLAGS=-I./include -O3
LFLAGS=-lSDL2 -lm

UNAME=$(shell uname)
ifeq ($(UNAME), Darwin)
	CC=clang
else
	CC=gcc
endif

SRC_DIR=./src
BUILD_DIR=./build
OBJ_DIR=$(BUILD_DIR)/obj
OBJS=$(subst .c,.o,$(subst $(SRC_DIR),$(OBJ_DIR),\
	 $(wildcard $(SRC_DIR)/*.c)))
DEP=$(subst .o,.d,$(OBJS))

BIN=app

all: $(BIN)

run: all
	$(BUILD_DIR)/$(BIN)

$(BIN): prepare $(BUILD_DIR)/$(BIN)

$(BUILD_DIR)/$(BIN): $(OBJS)
	$(CC) $(LFLAGS) $(DFLAGS) $^ -o $@
	
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(DFLAGS) -MMD -c $< -o $@

prepare:
	@mkdir -p $(OBJ_DIR)
	
debug: DFLAGS=-g -DDEBUG
debug: clean all
ifeq ($(UNAME), Darwin)
	dsymutil $(BUILD_DIR)/$(BIN)
endif
	
.PHONY: clean
	
clean:
	@rm -rf $(DEP) $(OBJS) $(BUILD_DIR)/$(BIN) $(BUILD_DIR)/$(BIN).dSYM

-include $(DEP)
