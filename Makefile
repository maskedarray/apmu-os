# Makefile for RISC-V ELF compilation and linking

# Compiler and flags
# CC = /home/a26rahma/work/alsaqr/llvm-testing/gcc-stuff/riscv-gnu-toolchain/output/bin/riscv32-unknown-elf-gcc
# LD = /home/a26rahma/work/alsaqr/llvm-testing/gcc-stuff/riscv-gnu-toolchain/output/bin/riscv32-unknown-elf-ld
# OBJDUMP = /home/a26rahma/work/alsaqr/llvm-testing/gcc-stuff/riscv-gnu-toolchain/output/bin/riscv32-unknown-elf-objdump
# OBJCOPY = /home/a26rahma/work/alsaqr/llvm-testing/gcc-stuff/riscv-gnu-toolchain/output/bin/riscv32-unknown-elf-objcopy
# CFLAGS = -march=rv32im_zicsr -mabi=ilp32 -mcmodel=medlow \
#          -Wall -fvisibility=hidden -ffreestanding \
#          -nostartfiles -O2 -specs=nano.specs -Wl,--gc-sections -flto -I common/include

CC = /home/a26rahma/work/alsaqr/llvm-testing/llvm-project/build/bin/clang
OBJDUMP = /home/a26rahma/work/alsaqr/llvm-testing/llvm-project/build/bin/llvm-objdump
OBJCOPY = /home/a26rahma/work/alsaqr/llvm-testing/llvm-project/build/bin/llvm-objcopy
CFLAGS = --target=riscv32-unknown-elf --sysroot=/home/a26rahma/work/alsaqr/final_testing_mohamed/riscv-toolchains/downloaded/lowrisc-riscv32/riscv32-unknown-elf -L/home/a26rahma/work/alsaqr/final_testing_mohamed/riscv-toolchains/downloaded/lowrisc-riscv32/riscv32-unknown-elf/lib -march=rv32im_zicsr -mabi=ilp32 -mcmodel=medlow \
         -Wall -fvisibility=hidden -ffreestanding \
         -O2 -nostdlib -flto -I common/include -I /home/a26rahma/work/alsaqr/final_testing_mohamed/riscv-toolchains/downloaded/lowrisc-riscv32/riscv32-unknown-elf/include -I common/include -I demos/include


# CC = clang
# OBJDUMP = llvm-objdump
# OBJCOPY = llvm-objcopy
# CFLAGS = --target=riscv32-unknown-elf --gcc-toolchain=/home/a26rahma/work/alsaqr/llvm-testing/gcc-stuff/lowrisc-toolchain-gcc-rv32imcb-20230811-1/ -march=rv32imc -mabi=ilp32 -mcmodel=medany \
#          -Wall -fvisibility=hidden -ffreestanding \
#          -nostartfiles -O0 -ggdb3 -I common/include 





# Source directory
SRC_DIR = .
COMMON_SRC_DIR = ./common
DEMOS_SRC_DIR = ./demos

# Source files
SRCS = $(SRC_DIR)/crt0.s $(SRC_DIR)/main.c $(wildcard $(COMMON_SRC_DIR)/*.c) $(wildcard $(DEMOS_SRC_DIR)/*.c)

# Linker script
CFLAGS += -I/home/a26rahma/work/alsaqr/llvm-testing/gcc-stuff/riscv-gnu-toolchain/build-newlib-nano/riscv32-unknown-elf/newlib/targ-include
LDSCRIPT = $(SRC_DIR)/linker.ld
LDFLAGS = -T $(LDSCRIPT) -L/home/a26rahma/work/alsaqr/llvm-testing/gcc-stuff/riscv-gnu-toolchain/build-newlib-nano/riscv32-unknown-elf/newlib -lnosys -lc 
LDFLAGS += /home/a26rahma/work/alsaqr/llvm-testing/gcc-stuff/riscv-gnu-toolchain/output/lib/gcc/riscv32-unknown-elf/13.2.0/libgcc.a

# Build directory
BUILD_DIR = build

# Output file
TARGET = $(BUILD_DIR)/output.elf

# Assembly dump file
ASM_FILE_DISASM = $(BUILD_DIR)/output.asm
ASM_FILE = $(BUILD_DIR)/main.s
OBJ_FILES = $(BUILD_DIR)/crt0.o $(BUILD_DIR)/main.o $(patsubst $(COMMON_SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(wildcard $(COMMON_SRC_DIR)/*.c)) $(patsubst $(DEMOS_SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(wildcard $(DEMOS_SRC_DIR)/*.c))

# Binary file
BIN_FILE = $(BUILD_DIR)/output.bin

# Custom instruction modification script
MODIFY_SCRIPT = Implinstr.py

.PHONY: all clean dump print_sizes

all: $(TARGET) dump print_sizes

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/crt0.o: $(SRC_DIR)/crt0.s | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Modify the .s files



# Compile common .c files to .o
$(BUILD_DIR)/%.o: $(COMMON_SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile common .c files to .o
$(BUILD_DIR)/%.o: $(DEMOS_SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile .s to .o after modification
compile_asm: $(BUILD_DIR)/crt0.o $(BUILD_DIR)/main.o $(patsubst $(COMMON_SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(wildcard $(COMMON_SRC_DIR)/*.c)) $(patsubst $(DEMOS_SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(wildcard $(DEMOS_SRC_DIR)/*.c))

# Link object files to create ELF
$(TARGET): compile_asm | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ_FILES) -o $@

clean:
	rm -rf $(BUILD_DIR)

dump: $(TARGET)
	$(OBJDUMP) -D $(TARGET) > $(ASM_FILE_DISASM)
	$(OBJCOPY) -O binary $(TARGET) $(BIN_FILE)

generate-bin-files: $(TARGET)
	$(OBJCOPY) -O binary --only-section=.text $(TARGET) build/text_section.bin
	$(OBJCOPY) -O binary --only-section=.data --only-section=.rodata --only-section=.bss $(TARGET) build/data_rodata_bss.bin


generate-assembly: $(BUILD_DIR)
	$(CC) $(CFLAGS) -S src/main.c -o $(BUILD_DIR)/main.S

print_sizes: $(TARGET)
	@TEXT_SIZE=$$($(OBJDUMP) -h $(TARGET) | awk '$$2 == ".text" { print $$3 }'); \
	DATA_SIZE=$$($(OBJDUMP) -h $(TARGET) | awk '$$2 == ".data" { print $$3 }'); \
	RODATA_SIZE=$$($(OBJDUMP) -h $(TARGET) | awk '$$2 == ".rodata" { print $$3 }'); \
	BSS_SIZE=$$($(OBJDUMP) -h $(TARGET) | awk '$$2 == ".bss" { print $$3 }'); \
	TOTAL_DATA_SIZE=$$(echo $$((0x$$DATA_SIZE + 0x$$RODATA_SIZE + 0x$$BSS_SIZE))); \
	echo "Text Section Size: $$((0x$$TEXT_SIZE))"; \
	echo "Data + Rodata + BSS Size: $$TOTAL_DATA_SIZE";

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

