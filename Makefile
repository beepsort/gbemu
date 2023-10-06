# tool macros
CC := g++
CCFLAGS := --std=c++17 `sdl2-config --cflags`
DBGFLAGS := -g
CCOBJFLAGS := -c $(CCFLAGS)
LDFLAGS := `sdl2-config --libs`
LDFLAGS_TEST := -lgtest -lgtest_main

# path macros
BIN_PATH := bin
OBJ_PATH := obj
SRC_PATH := src
DBG_PATH := debug

# compile macros
TARGET_NAME := gbemu
TEST_TARGET_NAME := gbemu_test
ifeq ($(OS),Windows_NT)
	TARGET_NAME := $(addsuffix .exe,$(TARGET_NAME))
endif
TARGET := $(BIN_PATH)/$(TARGET_NAME)
TARGET_DEBUG := $(DBG_PATH)/$(TARGET_NAME)
TARGET_TEST := $(DBG_PATH)/$(TEST_TARGET_NAME)

# src files & obj files
SRC := $(foreach x, $(SRC_PATH), $(wildcard $(addprefix $(x)/*,.c*)))
OBJ := $(addprefix $(OBJ_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))
OBJ_DEBUG := $(addprefix $(DBG_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))
SRC_TEST := $(filter-out $(SRC_PATH)/main.cpp, $(SRC))
OBJ_TEST := $(filter-out $(DBG_PATH)/main.o, $(OBJ_DEBUG))
SRC_TESTSUITES := $(foreach x, $(SRC_PATH), $(wildcard $(addprefix $(x)/tests/*,.c*)))
OBJ_TESTSUITES := $(addprefix $(DBG_PATH)/tests/, $(addsuffix .o, $(notdir $(basename $(SRC_TESTSUITES)))))

# clean files list
DISTCLEAN_LIST := $(OBJ) \
                  $(OBJ_DEBUG) \
				  $(OBJ_TEST) \
				  $(OBJ_TESTSUITES)
CLEAN_LIST := $(TARGET) \
			  $(TARGET_DEBUG) \
			  $(TARGET_TEST) \
			  $(DISTCLEAN_LIST)

# default rule
default: makedir all

# non-phony targets
$(TARGET): $(OBJ)
	$(CC) $(OBJ) $(CCFLAGS) $(LDFLAGS) -o $@

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c*
	$(CC) $< $(CCOBJFLAGS) -o $@

$(TARGET_DEBUG): $(OBJ_DEBUG)
	$(CC) $(OBJ_DEBUG) $(DBGFLAGS) $(CCFLAGS) $(LDFLAGS) -o $@

$(DBG_PATH)/%.o: $(SRC_PATH)/%.c*
	$(CC) $< $(DBGFLAGS) $(CCOBJFLAGS) -o $@

$(TARGET_TEST): $(OBJ_TEST) $(OBJ_TESTSUITES) #$(DBG_PATH)/tests/cpu_instruction_alu_test.o 
	$(CC) $(OBJ_TEST) $(OBJ_TESTSUITES) $(DBGFLAGS) $(CCFLAGS) $(LDFLAGS) $(LDFLAGS_TEST) -o $@

$(DBG_PATH)/tests/%.o: $(SRC_PATH)/tests/%.c*
	$(CC) $< $(DBGFLAGS) $(CCOBJFLAGS) -o $@

# phony rules
.PHONY: makedir
makedir:
	@mkdir -p $(BIN_PATH) $(OBJ_PATH) $(DBG_PATH) $(DBG_PATH)/tests

.PHONY: all
all: $(TARGET)

.PHONY: debug
debug: $(TARGET_DEBUG)

.PHONY: test
test: $(TARGET_TEST)
	@$(TARGET_TEST)

.PHONY: run
run:
	@$(TARGET)

.PHONY: clean
clean:
	@echo CLEAN $(CLEAN_LIST)
	@rm -f $(CLEAN_LIST)

.PHONY: distclean
distclean:
	@echo CLEAN $(DISTCLEAN_LIST)
	@rm -f $(DISTCLEAN_LIST)

.PHONY: printvars
printvars:
	@printf "\
	OBJ_DEBUG: \"$(OBJ_DEBUG)\"\n\
	OBJ_TEST: \"$(OBJ_TEST)\"\n\
	OBJ_TESTSUITES: \"$(OBJ_TESTSUITES)\"\n"
