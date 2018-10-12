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
	g++-7 f0_test_rcc.cpp $(CPP_FLAGS) -o ./$(BUILD_DIR)/f0_test_rcc -pthread
	./$(BUILD_DIR)/f0_test_rcc

$(BUILD_DIR):
	mkdir $@