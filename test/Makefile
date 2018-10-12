INCLUDES  =
INCLUDES += -I../src
INCLUDES += -I../src/bits
INCLUDES += -I../src/periph

INCLUDES_F0  =
INCLUDES_F0 += -I../STM32F0_files
INCLUDES_F0 += -I../STM32F0_files/CMSIS

CPP_FLAGS = $(INCLUDES) $(INCLUDES_F0) -std=c++17 -DTEST -g -Wno-register
BUILD_DIR = build

all: $(BUILD_DIR)
	g++-7 rcc_f0_test.cpp $(CPP_FLAGS) -o ./$(BUILD_DIR)/rcc_f0_test -pthread
	./$(BUILD_DIR)/rcc_f0_test

$(BUILD_DIR):
	mkdir $@