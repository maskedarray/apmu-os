# Makefile for RISC-V ELF compilation and linking

# Compiler and flags
CC = riscv32-unknown-elf-gcc
OBJDUMP = riscv32-unknown-elf-objdump
OBJCOPY = riscv32-unknown-elf-objcopy
CFLAGS = -march=rv32imcb -mabi=ilp32 -mcmodel=medany \
         -Wall -fvisibility=hidden -ffreestanding \
         -nostartfiles -O0 -ggdb3 -I common/include

# Source directory
SRC_DIR = .
COMMON_SRC_DIR = ./common

# Source files
SRCS = $(SRC_DIR)/crt0.s $(SRC_DIR)/main.c $(wildcard $(COMMON_SRC_DIR)/*.c)

# Linker script
LDSCRIPT = $(SRC_DIR)/linker.ld
LDFLAGS = -T $(LDSCRIPT)

# Build directory
BUILD_DIR = build

# Output file
TARGET = $(BUILD_DIR)/output.elf

# Assembly dump file
ASM_FILE_DISASM = $(BUILD_DIR)/output.asm
ASM_FILE = $(BUILD_DIR)/main.s
OBJ_FILES = $(BUILD_DIR)/crt0.o $(BUILD_DIR)/main.o $(patsubst $(COMMON_SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(wildcard $(COMMON_SRC_DIR)/*.c))

# Binary file
BIN_FILE = $(BUILD_DIR)/output.bin

# Custom instruction modification script
MODIFY_SCRIPT = Implinstr.py

.PHONY: all clean dump 

all: $(TARGET) dump

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/crt0.o: $(SRC_DIR)/crt0.s | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/main.s: $(SRC_DIR)/main.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -S $< -o $@

$(BUILD_DIR)/main.o: $(BUILD_DIR)/main.s | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Modify the .s files



# Compile common .c files to .o
$(BUILD_DIR)/%.o: $(COMMON_SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile .s to .o after modification
compile_asm: $(BUILD_DIR)/crt0.o $(BUILD_DIR)/main.o $(patsubst $(COMMON_SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(wildcard $(COMMON_SRC_DIR)/*.c))

# Link object files to create ELF
$(TARGET): compile_asm | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ_FILES) -o $@

clean:
	rm -rf $(BUILD_DIR)

dump: $(TARGET)
	$(OBJDUMP) -d $(TARGET) > $(ASM_FILE_DISASM)
	$(OBJCOPY) -O binary $(TARGET) $(BIN_FILE)

generate-assembly: $(BUILD_DIR)
	$(CC) $(CFLAGS) -S src/main.c -o $(BUILD_DIR)/main.S

#qemu single core emulation.
qemu-single-core: clean all
	qemu-system-riscv32 -nographic -machine virt -bios none -kernel build/output.elf 

qemu-single-core-debug: clean all
	qemu-system-riscv32 -nographic -machine virt -bios none -kernel build/output.elf -S -s


TARGET_CORE0 = $(BUILD_DIR)/core0/output0.elf
TARGET_CORE1 = $(BUILD_DIR)/core1/output1.elf

# Core 0 target
core0: $(TARGET_CORE0)
$(TARGET_CORE0): $(BUILD_DIR)
	make -C core0 all

# Core 1 target
core1: $(TARGET_CORE1)
$(TARGET_CORE1): $(BUILD_DIR)
	make -C core1 all

qemu-multicore: clean core0 core1
	qemu-system-riscv32 \
		-nographic \
		-machine virt -cpu rv32 -m 4G \
		-smp 2 \
		-bios none -device loader,file=build/core0/output0.elf,cpu-num=0 -device loader,file=build/core1/output1.elf,cpu-num=1 

qemu-multicore-debug: clean core0 core1
	qemu-system-riscv32 \
		-nographic \
		-machine virt -cpu rv32 -m 4G \
		-smp 2 \
		-bios none -device loader,file=build/core0/output0.elf,cpu-num=0 -device loader,file=build/core1/output1.elf,cpu-num=1 -S -s

