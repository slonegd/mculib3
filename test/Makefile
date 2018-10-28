INCLUDES  =
INCLUDES += -I../src
INCLUDES += -I../src/bits
INCLUDES += -I../src/periph

INCLUDES_F0  =
INCLUDES_F0 += -I../STM32F0_files
INCLUDES_F0 += -I../STM32F0_files/CMSIS

INCLUDES_F1  =
INCLUDES_F1 += -I../STM32F1_files
INCLUDES_F1 += -I../STM32F1_files/CMSIS

INCLUDES_F4  =
INCLUDES_F4 += -I../STM32F4_files
INCLUDES_F4 += -I../STM32F4_files/CMSIS

CPP_FLAGS = $(INCLUDES) -std=c++17 -DTEST -g -Wno-register
CPP_FLAGS_F0 += $(CPP_FLAGS) $(INCLUDES_F0)
CPP_FLAGS_F1 += $(CPP_FLAGS) $(INCLUDES_F1)
CPP_FLAGS_F4 += $(CPP_FLAGS) $(INCLUDES_F4)

BUILD_DIR = build

all: $(BUILD_DIR)
	g++-7 f0_test_rcc.cpp   $(CPP_FLAGS_F0) -o ./$(BUILD_DIR)/f0_test_rcc   -pthread
	g++-7 f1_test_rcc.cpp   $(CPP_FLAGS_F1) -o ./$(BUILD_DIR)/f1_test_rcc   -pthread
	g++-7 f1_test_gpio.cpp  $(CPP_FLAGS_F1) -o ./$(BUILD_DIR)/f1_test_gpio
	g++-7 f1_test_afio.cpp  $(CPP_FLAGS_F1) -o ./$(BUILD_DIR)/f1_test_afio
	g++-7 f4_test_usart.cpp $(CPP_FLAGS_F4) -o ./$(BUILD_DIR)/f4_test_usart
	g++-7 f4_test_flash.cpp $(CPP_FLAGS_F4) -o ./$(BUILD_DIR)/f4_test_flash -pthread

	./$(BUILD_DIR)/f0_test_rcc
	./$(BUILD_DIR)/f1_test_rcc
	./$(BUILD_DIR)/f1_test_gpio
	./$(BUILD_DIR)/f1_test_afio
	./$(BUILD_DIR)/f4_test_usart
	./$(BUILD_DIR)/f4_test_flash

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

cmake: clean
	cmake -H. -Bcmake
	cmake --build cmake
	cmake --build cmake --target test

clean:
	-rm -fR .dep $(BUILD_DIR)

current: $(BUILD_DIR)
	g++-7 f1_test_afio.cpp  $(CPP_FLAGS_F1) -o ./$(BUILD_DIR)/f1_test_afio
	./$(BUILD_DIR)/f1_test_afio