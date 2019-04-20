TARGET_F1 = main
DEBUG = 1
OPT = -Os
CPPSTD =-std=c++17
BUILD_DIR = build

######################################
# source
######################################
CPP_SOURCES_F1 = main.cpp
CMSIS_PATH = ../../STM32F1_files
LIBRARY_PATH = ../..

ASM_SOURCES_F1 = $(CMSIS_PATH)/startup_stm32f103xb.s
LDSCRIPT_F1 = $(CMSIS_PATH)/STM32F103RBTx_FLASH.ld

# C includes
C_INCLUDES =  
C_INCLUDES += -I.
C_INCLUDES += -I$(CMSIS_PATH)
C_INCLUDES += -I$(CMSIS_PATH)/CMSIS
C_INCLUDES += -I$(LIBRARY_PATH)/src
C_INCLUDES += -I$(LIBRARY_PATH)/src/periph
C_INCLUDES += -I$(LIBRARY_PATH)/src/bits
C_INCLUDES += -I$(LIBRARY_PATH)/src/example
C_INCLUDES += -I$(LIBRARY_PATH)/src/middleware


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
CPU_F1 = -mcpu=cortex-m0

# NONE for Cortex-M0/M0+/M3
FPU_F1 =

FLOAT-ABI_F1 =

# mcu
MCU_F1 = $(CPU_F1) -mthumb $(FPU_F1) $(FLOAT-ABI_F1)

# compile gcc flags
ASFLAGS_F1 = $(MCU_F1) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS_F1  = $(MCU_F1) $(C_DEFS_F1) $(C_INCLUDES) $(C_INCLUDES_F1) $(OPT)
CFLAGS_F1 += -Wall -Wno-register -Wno-strict-aliasing -fdata-sections -ffunction-sections -fno-exceptions -fno-strict-volatile-bitfields -fexec-charset=cp1251
CFLAGS_F1 += -g -gdwarf-2 


# Generate dependency information
CFLAGS_F1 += -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)"

#######################################
# LDFLAGS
#######################################
# libraries
LIBS = -lc -lm -lnosys

LDFLAGS_F1  = $(MCU_F1) -specs=nano.specs -specs=nosys.specs
LDFLAGS_F1 += -T$(LDSCRIPT_F1) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET_F1).map,--cref -Wl,--gc-sections

# default action: build all
all: clean \
$(BUILD_DIR)/$(TARGET_F1).elf $(BUILD_DIR)/$(TARGET_F1).hex $(BUILD_DIR)/$(TARGET_F1).bin
	


#######################################
# build the application
#######################################
# list of objects
OBJECTS_F1 += $(addprefix $(BUILD_DIR)/,$(notdir $(CPP_SOURCES_F1:.cpp=.o)))
vpath %.cpp $(sort $(dir $(CPP_SOURCES_F1)))
OBJECTS_F1 += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES_F1:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES_F1)))



$(BUILD_DIR)/$(TARGET_F1).o:$(CPP_SOURCES_F1) Makefile | $(BUILD_DIR) 
	$(CPP) -c $(CFLAGS_F1) $(CPPSTD) -fno-rtti -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.cpp=.lst)) $< -o $@

$(BUILD_DIR)/startup_stm32f103xb.o: $(ASM_SOURCES_F1) Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS_F1) $< -o $@

$(BUILD_DIR)/$(TARGET_F1).elf: $(OBJECTS_F1) Makefile
	$(CPP) $(OBJECTS_F1) $(LDFLAGS_F1) -o $@
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
	st-flash write $(BUILD_DIR)/$(TARGET_F1).bin 0x8000000

util:
	st-util

test_:
	$(MAKE) -C ./test/
  
#######################################
# dependencies
#######################################
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)

# *** EOF ***