# Compiler and flags
CC = riscv32-unknown-elf-gcc
CC_PREFIX = riscv32-unknown-elf-
CFLAGS = -march=rv32im -mabi=ilp32 -mcmodel=medany \
         -Wall -fvisibility=hidden -ffreestanding \
         -nostartfiles -I../../common/include -O0 -ggdb3

# Directories
BUILD_DIR = ../../build/core1
SRC_DIR = .
COMMON_DIR = ../../common
COMMON_SRC_DIR = $(COMMON_DIR)

# Source files
SRCS = $(SRC_DIR)/main1.c ../../crt0.s $(COMMON_SRC_DIR)/queue_lib.c

# Linker script
LDSCRIPT = $(SRC_DIR)/linker.ld
LDFLAGS = -T $(LDSCRIPT)

# Output files
TARGET = $(BUILD_DIR)/output1.elf
ASM_FILE_DISASM = $(BUILD_DIR)/output1.asm
OBJ_FILES = $(BUILD_DIR)/crt0.o $(BUILD_DIR)/main1.o $(BUILD_DIR)/queue_lib.o

.PHONY: all clean

all: $(TARGET) dump

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/crt0.o: ../../crt0.s | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/main1.o: $(SRC_DIR)/main1.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/queue_lib.o: $(COMMON_SRC_DIR)/queue_lib.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files to create ELF
$(TARGET): $(BUILD_DIR)/crt0.o $(BUILD_DIR)/main1.o $(BUILD_DIR)/queue_lib.o | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ_FILES) -o $@

dump: $(TARGET)
	@echo "Generating assembly dump for Core1..."
	$(CC_PREFIX)objdump -d --disassemble-all --debugging $(TARGET) > $(BUILD_DIR)/output1.asm

clean:
	rm -rf $(BUILD_DIR)
