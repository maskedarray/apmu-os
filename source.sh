# apmu-os build environment.
#
#   source source.sh && make generate-bin-files
#
# The Makefile hard-errors if any of these four are unset, so it will tell you
# immediately if you forgot to source this. Everything is resolved relative to
# this file, so the paths follow the checkout rather than being pinned to one
# machine (the note at the top of main.c points at a lab box that is not here).

# Directory containing this script, whether sourced from bash or zsh.
_src="${BASH_SOURCE[0]:-$0}"
APMU_OS_DIR="$(cd "$(dirname "$_src")" && pwd)"
UW_ROOT="$(cd "$APMU_OS_DIR/.." && pwd)"
unset _src

# clang, llvm-objcopy, llvm-objdump. This is a full LLVM build tree, not an
# install prefix -- the binaries sit directly in build/bin.
export LLVM_BIN="$UW_ROOT/llvm-project/build/bin"

# riscv32-unknown-elf sysroot: supplies the headers, and the -L that resolves
# -lnosys (libnosys.a lives in its lib/, not in NEWLIB_BUILD).
export RISCV_SYSROOT="$UW_ROOT/riscv-gnu-toolchain/output/riscv32-unknown-elf"

# newlib-nano build tree, for libc.a and targ-include. This is the top-level
# (default multilib) build, which is what the Makefile documents and what has
# built this project before. An exact match for -march=rv32im_zicsr exists at
# .../riscv32-unknown-elf/rv32im/ilp32/newlib if you ever want to switch; a
# subset-ISA libc links fine into rv32im code, so the default is not a problem.
export NEWLIB_BUILD="$UW_ROOT/riscv-gnu-toolchain/build-newlib-nano/riscv32-unknown-elf/newlib"

# libgcc must match -march=rv32im_zicsr -mabi=ilp32, so pick the rv32im/ilp32
# multilib explicitly -- clang is not a gcc driver and will not select one for
# us. The version directory is globbed so a toolchain rebuild does not break
# this. The other multilibs here include rv64 variants, which would fail to
# link rather than fail quietly.
LIBGCC_A="$(echo "$UW_ROOT"/riscv-gnu-toolchain/output/lib/gcc/riscv32-unknown-elf/*/rv32im/ilp32/libgcc.a | tr ' ' '\n' | tail -1)"
export LIBGCC_A

# The LLVM and RISC-V toolchains in this tree are Linux aarch64 binaries, built
# inside the OrbStack VM. /Users/abdur is shared into the VM at the same path,
# so these variables are correct on both sides -- but make(1) only works on the
# Linux side. On macOS clang dies with "cannot execute binary file".
if [ "$(uname -s)" = "Darwin" ]; then
    echo "source.sh: note -- the toolchain here is Linux aarch64, so build inside" >&2
    echo "           the VM:  orb -m uw-research" >&2
    echo "           (paths are identical on both sides; only make must run there)" >&2
fi

# Fail loudly here rather than halfway through a link.
_missing=0
for _v in LLVM_BIN RISCV_SYSROOT NEWLIB_BUILD LIBGCC_A; do
    eval "_p=\$$_v"
    if [ ! -e "$_p" ]; then
        echo "source.sh: $_v does not exist: $_p" >&2
        _missing=1
    fi
done
for _f in "$LLVM_BIN/clang" "$LLVM_BIN/llvm-objcopy" "$NEWLIB_BUILD/libc.a" \
          "$RISCV_SYSROOT/lib/libnosys.a"; do
    if [ ! -e "$_f" ]; then
        echo "source.sh: missing $_f" >&2
        _missing=1
    fi
done
if [ "$_missing" = 0 ]; then
    echo "apmu-os environment ready (LLVM_BIN=$LLVM_BIN)"
fi
unset _missing _v _p _f
