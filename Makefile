CC = i686-elf-g++
LD = i686-elf-ld
NASM = nasm

BIN = LittleOS.bin
CFLAGS  := -g -ffreestanding -nostdlib -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -O0 -falign-jumps -falign-functions -falign-labels -fomit-frame-pointer -finline-functions -falign-loops -fstrength-reduce -Wno-unused-function -Wno-unused-parameter -fno-builtin -Wno-unused-label -Wno-cpp -fno-rtti -fno-exceptions -Iinc
LDFLAGS := -g -relocatable
LIBS 	:= 
INCLUDES = -I.

LIBDIR	= 

BUILDDIR  = Build
ARCHDIR   = Kernel/Arch/x86
KERNELDIR = Kernel
DRIVERSDIR = Kernel/Drivers

include $(ARCHDIR)/make.config
include $(KERNELDIR)/make.config
include $(DRIVERSDIR)/make.config

CFLAGS 	:= $(CFLAGS) $(LIBDIR)

OBJS=\
$(ARCH_OBJS) \
$(KERNEL_OBJS) \
$(DRIVERS_OBJS) \

OBJS_OUT := $(foreach item,$(OBJS),$(BUILDDIR)/$(item))

.PHONY: all clean
.SUFFIXES: .o .c .asm.o .asm

all: $(BIN)

$(BIN): $(OBJS) $(ARCHDIR)/linker.ld
	$(NASM) -f bin $(ARCHDIR)/Boot.asm -o $(BUILDDIR)/$(ARCHDIR)/Boot.bin
	$(LD) $(LDFLAGS) $(OBJS_OUT) -o $(BUILDDIR)/KernelFull.o
	$(CC) -T $(ARCHDIR)/linker.ld -o $(BUILDDIR)/Kernel.bin $(CFLAGS) $(BUILDDIR)/KernelFull.o $(LIBS)

	rm -rf $(BUILDDIR)/$(BIN)
	dd if=$(BUILDDIR)/$(ARCHDIR)/Boot.bin >> $(BUILDDIR)/$(BIN)
	dd if=$(BUILDDIR)/Kernel.bin >> $(BUILDDIR)/$(BIN)
	dd if=/dev/zero bs=512 count=100 >> $(BUILDDIR)/$(BIN)

.cpp.o:
	@mkdir -p $(BUILDDIR)/$(@D)
	$(CC) -c $< -o $(BUILDDIR)/$@ -std=c++17 $(CFLAGS) $(INCLUDES)

%.asm.o: %.asm
	@mkdir -p $(BUILDDIR)/$(@D)
	$(NASM) -f elf -g $< -o $(BUILDDIR)/$@

clean:
	rm -f $(BUILDDIR)/$(BIN)
	rm -f $(OBJS) *.o */*.o */*/*.o
	rm -f $(OBJS:.o=.d) *.d */*.d */*/*.d
	rm -f $(OBJS:.o=.bin) *.bin */*.bin */*/*.bin
	rm -rf $(BUILDDIR)

run:
	qemu-system-i386 -hda $(BUILDDIR)/$(BIN)

-include $(OBJS:.o=.d)
