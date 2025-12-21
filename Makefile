CC = x86_64-elf-gcc
LD = x86_64-elf-gcc
AS = nasm

MODE ?= vga

CFLAGS_COMMON = -ffreestanding -m64 -mcmodel=kernel \
                -fno-stack-protector -fno-pic -fno-pie \
                -mno-red-zone -Ikernel
LDFLAGS = -ffreestanding -m64 -nostdlib -T linker.ld
ifeq ($(MODE),serial)
CFLAGS = $(CFLAGS_COMMON) -DSERIAL
else ifeq ($(MODE),vga)
CFLAGS = $(CFLAGS_COMMON) -DVGA
else ifeq ($(MODE),both)
CFLAGS = $(CFLAGS_COMMON) -DSERIAL -DVGA
else
$(error Unknown MODE "$(MODE)")
endif

KERNEL = kernel.elf
ISO = Eleuther-64.iso
ISO_DIR = iso

C_SRCS = \
	kernel/kernel.c \
	kernel/serial/serial.c \
	kernel/crash/crash.c \
	kernel/itoa/itoa.c \
	kernel/pic/pic.c \
	kernel/idt/idt.c \
	kernel/timer/timer.c \
	kernel/vga/vga.c

ASM_SRCS = \
	boot.asm \
	kernel/idt/interrupt.asm 

OBJS = $(C_SRCS:.c=.o) $(ASM_SRCS:.asm=.o)

all: $(KERNEL)

$(KERNEL): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.asm
	$(AS) -f elf64 $< -o $@

iso: $(KERNEL)
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(KERNEL) $(ISO_DIR)/boot/
	cp grub.cfg $(ISO_DIR)/boot/grub/
	grub-mkrescue -o $(ISO) $(ISO_DIR)

run: iso
	qemu-system-x86_64 \
		-cdrom $(ISO) \
		-serial stdio \
		-audiodev pa,id=snd \
		-machine pcspk-audiodev=snd

clean:
	rm -rf $(OBJS) $(KERNEL) $(ISO)