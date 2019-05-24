TARGET_F4 = main
DEBUG = 1
OPT = -O0
CPPSTD =-std=c++17
BUILD_DIR = build

######################################
# source
######################################
CPP_SOURCES_F4 = main.cpp
CMSIS_PATH = ../../STM32F4_files
LIBRARY_PATH = ../..

ASM_SOURCES_F4 = $(CMSIS_PATH)/startup_stm32f405xx.s
LDSCRIPT_F4 = $(CMSIS_PATH)/STM32F405RGTx_FLASH.ld

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
CPU_F4 = -mcpu=cortex-m4
FPU_F4 = -mfpu=fpv4-sp-d16
FLOAT-ABI_F4 = -mfloat-abi=hard

# mcu
MCU_F4 = $(CPU_F4) -mthumb $(FPU_F4) $(FLOAT-ABI_F4)

# compile gcc flags
ASFLAGS_F4 = $(MCU_F4) $(OPT) -Wall -fdata-sections -ffunction-sections 

CFLAGS_F4  = $(MCU_F4) $(C_DEFS_F4) $(C_INCLUDES) $(C_INCLUDES_F4) $(OPT)
CFLAGS_F4 += -Wall -Wno-register -Wno-strict-aliasing -fdata-sections -ffunction-sections -fno-exceptions -fno-strict-volatile-bitfields -fno-threadsafe-statics -Wno-packed-bitfield-compat
CFLAGS_F4 += -g -gdwarf-2 


# Generate dependency information
CFLAGS_F4 += -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)"

#######################################
# LDFLAGS
#######################################
# libraries
LIBS = -lc -lm -lnosys

LDFLAGS_F4  = $(MCU_F4) -specs=nano.specs -specs=nosys.specs
LDFLAGS_F4 += -T$(LDSCRIPT_F4) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET_F4).map,--cref -Wl,--gc-sections

# default action: build all
all: clean \
$(BUILD_DIR)/$(TARGET_F4).elf $(BUILD_DIR)/$(TARGET_F4).hex $(BUILD_DIR)/$(TARGET_F4).bin
	


#######################################
# build the application
#######################################
# list of objects
OBJECTS_F4 += $(addprefix $(BUILD_DIR)/,$(notdir $(CPP_SOURCES_F4:.cpp=.o)))
vpath %.cpp $(sort $(dir $(CPP_SOURCES_F4)))
OBJECTS_F4 += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES_F4:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES_F4)))



$(BUILD_DIR)/$(TARGET_F4).o:$(CPP_SOURCES_F4) Makefile | $(BUILD_DIR) 
	$(CPP) -c $(CFLAGS_F4) $(CPPSTD) -fno-rtti -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.cpp=.lst)) $< -o $@

$(BUILD_DIR)/startup_stm32f405xx.o: $(ASM_SOURCES_F4) Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS_F4) $< -o $@

$(BUILD_DIR)/$(TARGET_F4).elf: $(OBJECTS_F4) Makefile
	$(CPP) $(OBJECTS_F4) $(LDFLAGS_F4) -o $@
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
	st-flash write $(BUILD_DIR)/$(TARGET_F4).bin 0x8000000

util:
	st-util

test_:
	$(MAKE) -C ./test/
  
#######################################
# dependencies
#######################################
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)

# *** EOF ***