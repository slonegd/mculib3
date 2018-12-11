TARGET_F7 = main
DEBUG = 1
OPT = -Os
CPPSTD =-std=c++17
BUILD_DIR = build

######################################
# source
######################################
CPP_SOURCES_F7 = main.cpp
CMSIS_PATH = ../../STM32F7_files
LIBRARY_PATH = ../..

ASM_SOURCES_F7 = $(CMSIS_PATH)/startup_stm32f746xx.s
LDSCRIPT_F7 = $(CMSIS_PATH)/STM32F746NGHx_FLASH.ld

# C includes
C_INCLUDES =  
C_INCLUDES += -I.
C_INCLUDES += -I$(CMSIS_PATH)
C_INCLUDES += -I$(CMSIS_PATH)/CMSIS
C_INCLUDES += -I$(LIBRARY_PATH)/src
C_INCLUDES += -I$(LIBRARY_PATH)/src/periph
C_INCLUDES += -I$(LIBRARY_PATH)/src/bits


#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-

CPP = $(PREFIX)g++
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
AR = $(PREFIX)ar
SZ = $(PREFIX)size
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
#######################################
# CFLAGS
#######################################
CPU_F7 = -mcpu=cortex-m7
FPU_F7 = -mfpu=fpv5-sp-d16
FLOAT-ABI_F7 = -mfloat-abi=hard

# mcu
MCU_F7 = $(CPU_F7) -mthumb $(FPU_F7) $(FLOAT-ABI_F7)

# compile gcc flags
ASFLAGS_F7 = $(MCU_F7) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS_F7  = $(MCU_F7) $(C_INCLUDES) $(OPT)
CFLAGS_F7 += -Wall -Wno-register -fdata-sections -ffunction-sections -fno-exceptions -fno-strict-volatile-bitfields
ifeq ($(DEBUG), 1)
CFLAGS_F7 += -g -gdwarf-2
endif


# Generate dependency information
CFLAGS_F7 += -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)"

#######################################
# LDFLAGS
#######################################
# libraries
LIBS = -lc -lm -lnosys

LDFLAGS_F7  = $(MCU_F7) -specs=nano.specs -specs=nosys.specs
LDFLAGS_F7 += -T$(LDSCRIPT_F7) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET_F7).map,--cref -Wl,--gc-sections

# default action: build all
all: clean \
$(BUILD_DIR)/$(TARGET_F7).elf $(BUILD_DIR)/$(TARGET_F7).hex $(BUILD_DIR)/$(TARGET_F7).bin
	


#######################################
# build the application
#######################################
# list of objects
OBJECTS_F7 += $(addprefix $(BUILD_DIR)/,$(notdir $(CPP_SOURCES_F7:.cpp=.o)))
vpath %.cpp $(sort $(dir $(CPP_SOURCES_F7)))
OBJECTS_F7 += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES_F7:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES_F7)))



$(BUILD_DIR)/$(TARGET_F7).o:$(CPP_SOURCES_F7) Makefile | $(BUILD_DIR) 
	$(CPP) -c $(CFLAGS_F7) $(CPPSTD) -fno-rtti -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.cpp=.lst)) $< -o $@

$(BUILD_DIR)/startup_stm32f746xx.o: $(ASM_SOURCES_F7) Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS_F7) $< -o $@

$(BUILD_DIR)/$(TARGET_F7).elf: $(OBJECTS_F7) Makefile
	$(CPP) $(OBJECTS_F7) $(LDFLAGS_F7) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@

clean:
	-rm -fR .dep $(BUILD_DIR)

flash:
	st-flash write $(BUILD_DIR)/$(TARGET_F7).bin 0x8000000

util:
	st-util

test_:
	$(MAKE) -C ./test/
  
#######################################
# dependencies
#######################################
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)

# *** EOF ***